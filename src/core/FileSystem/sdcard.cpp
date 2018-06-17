#include "../3rdParty/FatFs/diskio.h"
#include <stm32f7xx.h>
#include <thread>
#include <chrono>
#include <cstdint>
#include "../commdef.hpp"


constexpr uint8_t SD_INIT_CLKDIV = 178;		//48MHz CLKSDIO / (SD_INIT_CLKDIV + 2) = 400kHz
constexpr uint8_t SD_NORMAL_CLKDIV = 0;		//48MHz CLKSDIO / (SD_NORMAL_CLKDIV + 2) = 24MHz

using namespace std::chrono_literals;

using SDIOModule_t = SDMMC_TypeDef;

enum class SDCommand: uint8_t {
	CMD0 = 0,
	GO_IDLE_STATE = 0,
	CMD8 = 8,
	SEND_IF_COND = 8,
	CMD55 = 55,
	APP_CMD = 55,
	ACMD41 = 41,
	SD_SEND_OP_COND = 41,
	ACMD6 = 6,
	SET_BUS_WIDTH = 6,
	CMD2 = 2,
	ALL_SEND_CID = 2,
	CMD3 = 3,
	SEND_RELATIVE_ADDR = 3,
	CMD9 = 9,
	SEND_CSD = 9,
	CMD7 = 7,
	SELECT_DESELECT_CARD = 7,
	CMD24 = 24,
	WRITE_SINGLE_BLOCK = 24,
	CMD25 = 25,
	WRITE_MULTIPLE_BLOCK = 25,
	CMD12 = 12,
	STOP_TRANSMISSION = 12,
	CMD13 = 13,
	SEND_STATUS = 13,
	CMD17 = 17,
	READ_SINGLE_BLOCK = 17,
	CMD18 = 18,
	READ_MULTIPLE_BLOCK = 18,
};

enum class SDResponse: uint32_t {
	NO = 0,
	SHORT = (1 << 6),
	LONG = (3 << 6),
	R3 = 0xF40	//Note this is totaly out of standard. However, becouse of the masking in SD_Command it will be processed as SHRESP
				//R3 does not contain a valid CRC. Therefore, CCRCFAIL is set and CMDREND is never set for R3.
				//To properly process R3, exit the loop CCRCFAIL condition and don't check CMDREND
};

enum class SDState: uint32_t {
	IDLE,				// Card state is idle
	READY,				// Card state is ready
	IDENTIFICATION,		// Card is in identification state
	STANDBY,			// Card is in standby state
	TRANSFER,			// Card is in transfer state
	SENDING,			// Card is sending an operation
	RECEIVING,			// Card is receiving operation information
	PROGRAMMING,		// Card is in programming state
	DISCONNECTED,		// Card is disconnected
};

/*
==================
ClearFlags
==================
*/
static inline void ClearFlags(SDIOModule_t *module) {
	module->ICR = SDMMC_STA_CCRCFAIL | SDMMC_STA_DCRCFAIL | SDMMC_STA_CTIMEOUT | SDMMC_STA_DTIMEOUT | SDMMC_STA_TXUNDERR | SDMMC_STA_RXOVERR | SDMMC_STA_CMDREND | SDMMC_STA_CMDSENT | SDMMC_STA_DATAEND | SDMMC_STA_DBCKEND;
}

/*
==================
SendCommand

	Send command to card and wait for send and response, if need.
	NOTE without noclone and noinline attributes this function will be broken under optimizations!
==================
*/
[[using gnu: noclone, noinline]] static int SendCommand(SDIOModule_t *module, const SDCommand cmd, const uint32_t arg, const SDResponse response) {
	ClearFlags(module);

	module->ARG = arg; //First adjust the argument (because I will immediately enable CPSM next)
	__DSB();
	module->CMD = (uint32_t)(uint8_t(cmd) & SDMMC_CMD_CMDINDEX) | (uint32_t(response) & SDMMC_CMD_WAITRESP) | SDMMC_CMD_CPSMEN;
	__DSB();

	//Block till we get a response
	if (unlikely(response == SDResponse::NO)) {
		//We should wait for CMDSENT
		while (!(module->STA & (SDMMC_STA_CTIMEOUT | SDMMC_STA_CMDSENT)));
	} else {
		//We should wait for CMDREND or CCRCFAIL
		while (!(module->STA & (SDMMC_STA_CTIMEOUT | SDMMC_STA_CMDREND | SDMMC_STA_CCRCFAIL)));
		/*if (likely((response == SDResponse::SHORT) && !(module->STA & (SDIO_STA_CTIMEOUT | SDIO_STA_CCRCFAIL)))) {
			while (module->RESPCMD != uint32_t(cmd));
		}*/
	}

	//Check to see if the response is valid
	//We consider all R3 responses without a timeout as a valid response
	//It seems CMDSENT and CMDREND are mutually exlusive. (though I am not sure. Check this later)
	if (module->STA & SDMMC_STA_CTIMEOUT) {
		return 2;		//timeout
	} else if ((module->STA & SDMMC_STA_CCRCFAIL) && (response != SDResponse::R3)) {
		return 1;		//crc
	}

	return 0;			//success
}

struct CardInfo {
	bool initialized;
	unsigned int type;
	uint32_t RCA, PSN;
	uint64_t capacity;
	uint8_t MID, PRV;
	char OID[2], PNM[5];

	CardInfo(): initialized(false) {}

	void ParseCID(const uint32_t *cid) {
		MID = (cid[0] & 0xFF000000) >> 24;

		OID[0] = (cid[0] & 0x00FF0000) >> 16;
		OID[1] = (cid[0] & 0x0000FF00) >> 8;

		PNM[0] = cid[0] & 0x000000FF;
		PNM[1] = (cid[1] & 0xFF000000) >> 24;
		PNM[2] = (cid[1] & 0x00FF0000) >> 16;
		PNM[3] = (cid[1] & 0x0000FF00) >> 8;
		PNM[4] = cid[1] & 0x000000FF;

		PRV = cid[2] & 0xFF000000;

		PSN = ((cid[2] & 0x00FFFFFF) << 8) | ((cid[3] & 0xFF000000) >> 24);
	}

	void CalcCapacity(const uint32_t *csd) {
		unsigned int c_size;

		if (((csd[0] & 0xC0000000) >> 30) == 0b00) {			//CSD Version 1.0
			int c_size_mult, read_bl_len;

			c_size = ((csd[1] & 0x3FF) << 2) | ((csd[2] & 0xC0000000) >> 30);
			c_size_mult = (csd[2] & 0x38000) >> 15;
			read_bl_len = (csd[1] & 0xF0000) >> 16;
			capacity = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << read_bl_len);
		} else if (((csd[0] & 0xC0000000) >> 30) == 0b01) {		//CSD Version 2.0
			c_size = ((csd[1] & 0x3F) << 16) | ((csd[2] & 0xFFFF0000) >> 16);
			capacity = static_cast<uint64_t>((c_size + 1)) * 524288ULL;
		}
	}
};

static CardInfo g_bottomCardInfo, g_topCardInfo;

/*
==================
GetCardState
==================
*/
static DRESULT GetCardState(SDIOModule_t *module, const CardInfo &info, SDState &result) {
	if (SendCommand(module, SDCommand::SEND_STATUS, info.RCA << 16, SDResponse::SHORT)) {
		return RES_ERROR;
	}
	
	result = SDState((module->RESP1 >> 9) & 0xF);

	if (module->RESP1 & 0xFD'FF'E0'08) {
		return RES_ERROR;
	}

	return RES_OK;
}

/*
==================
InitSDCard
==================
*/
static DSTATUS InitSDCard(SDIOModule_t *module, CardInfo &infoOut, const bool useWideBus) {
	uint32_t longResp[4];

	module->CLKCR = SD_INIT_CLKDIV;			// this also disables clock for card
	module->POWER = SDMMC_POWER_PWRCTRL;

	std::this_thread::sleep_for(1ms);

	module->CLKCR |= SDMMC_CLKCR_CLKEN;		// enable clock for card

	if (SendCommand(module, SDCommand::GO_IDLE_STATE, 0, SDResponse::NO)) {
		return STA_NOINIT;
	}

	if (SendCommand(module, SDCommand::SEND_IF_COND, 0x1AA, SDResponse::SHORT) != 2) {	//Non v2.0 compliant sd's will cause panic here due to the timeout
		if ((module->RESP1 & 0xFF) != 0xAA) {		//AA is the check pattern.
			return STA_NOINIT;
		}
	}
	while (true) {
		if (SendCommand(module, SDCommand::APP_CMD, 0, SDResponse::SHORT)) {	//Note that argument should be RCA. But at this point RCA of SD is 0. (It will be changed after cmd3)
			return STA_NOINIT;
		}
		if (module->RESP1 & 0xFD'FF'E0'08) {		//All error bits must be zero
			return STA_NOINIT;
		}

		//ACMD41 (Response is R3 which does not contain any CRC)
		//Second argument in the argument indicates that host supports SDHC. We will check acmd41 response if the SD is SC or HC
		if (SendCommand(module, SDCommand::SD_SEND_OP_COND, 0x80'10'00'00 | 0x40'00'00'00, SDResponse::R3)) {
			return STA_NOINIT;
		}

		unsigned int timeOut = 0xFF'FF'FF'FF;

		//Check the ready status in the response (R3)
		if ((module->RESP1 >> 31) == 1) { //When card is busy this bit will be 0
			//Card is now initialized. Check to see if SD is SC or HC
			infoOut.type = (module->RESP1 & 0x40'00'00'00) >> 30; //1=HC, 0=SC
			break;
		} else {
			if (!(--timeOut)) {
				return STA_NOINIT;
			}
		}
	}

	//Now we are in the Ready State

	//Ask for CID using CMD2
	if (SendCommand(module, SDCommand::CMD2, 0, SDResponse::LONG)) {
		return STA_NOINIT;
	}
	longResp[0] = module->RESP1;		//[127:96]
	longResp[1] = module->RESP2;		//[95:64]
	longResp[2] = module->RESP3;		//[63:32]
	longResp[3] = module->RESP4;		//[31:1]0b
	infoOut.ParseCID(longResp);

	//Now the card is in the identification mode

	//Request for the RCA number with cmd3
	if (SendCommand(module, SDCommand::CMD3, 0, SDResponse::SHORT)) {
		return STA_NOINIT;
	}
	infoOut.RCA = module->RESP1 >> 16;		//Equals to OCR

	//Now the card is in stand-by mode

	if (SendCommand(module, SDCommand::CMD9, (infoOut.RCA << 16), SDResponse::LONG)) {		//Use cmd9 to read the card specific information
		return STA_NOINIT;
	}
	longResp[0] = module->RESP1;		//[127:96]
	longResp[1] = module->RESP2;		//[95:64]
	longResp[2] = module->RESP3;		//[63:32]
	longResp[3] = module->RESP4;		//[31:1]0b
	infoOut.CalcCapacity(longResp);

	//Put the Card in the transfer mode using cmd7

	//Bus width can only be changed in transfer mode
	if (SendCommand(module, SDCommand::CMD7, (infoOut.RCA << 16), SDResponse::SHORT)) {
		return STA_NOINIT;
	}
	if (module->RESP1 & 0xFD'FF'E0'08) {
		return STA_NOINIT;
	}

	if (useWideBus) {
		if (SendCommand(module, SDCommand::APP_CMD, (infoOut.RCA << 16), SDResponse::SHORT)) {		//Note the real RCA in the argument
			return STA_NOINIT;
		}
		if (module->RESP1 & 0xFD'FF'E0'08) {;
			return STA_NOINIT;
		}

		//Change the bus width with acmd6
		if (SendCommand(module, SDCommand::ACMD6, 0x02, SDResponse::SHORT)) {
			return STA_NOINIT;
		}
		if (module->RESP1 & 0xFD'FF'E0'08) {
			return STA_NOINIT;
		}

		module->CLKCR = SDMMC_CLKCR_WIDBUS_0 | SDMMC_CLKCR_PWRSAV | SDMMC_CLKCR_CLKEN | SD_NORMAL_CLKDIV;		// enable 4-bit bus, power save and normal clkdiv
	} else {
		module->CLKCR = SDMMC_CLKCR_PWRSAV | SDMMC_CLKCR_CLKEN | SD_NORMAL_CLKDIV;		//use 1 bit bus, select power safe and nornal clkdiv
	}

	infoOut.initialized = true;

	return 0;
}

/*
==================
ReadBlocks
==================
*/
static DRESULT ReadBlocks(SDIOModule_t *module, const CardInfo &cardInfo, BYTE* buff, DWORD sector, UINT count) {
	module->DTIMER = 0xFF'FF'FF'FF;
	module->DLEN = count * 512;
	module->DCTRL = (9 << 4) | SDMMC_DCTRL_DTDIR | SDMMC_DCTRL_DTEN;		//512 bytes from card

	uint32_t address = sector;
	if (cardInfo.type == 0) {	//Standart capacity
		address *= 512;
	}

	if (count > 1) {
		if (SendCommand(module, SDCommand::READ_MULTIPLE_BLOCK, address, SDResponse::SHORT)) {
			return RES_ERROR;
		}
	} else {
		if (SendCommand(module, SDCommand::READ_SINGLE_BLOCK, address, SDResponse::SHORT)) {
			return RES_ERROR;
		}
	}

	if (module->RESP1 & 0xFD'FF'E0'08) {
		return RES_ERROR;
	}

	ClearFlags(module);
	uint32_t *tmpBuff = reinterpret_cast<uint32_t*>(buff);

	// Read block(s) in polling mode
	while (!(module->STA & (SDMMC_STA_RXOVERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DATAEND))) {
		if (module->STA & SDMMC_STA_RXFIFOHF) {		// at least 8 words avialable in fifo
			for (int i = 0; i < 8; i++, tmpBuff++) {
				*tmpBuff = module->FIFO;
			}
		}
	}

	// Send stop transmission command in case of multiblock read
	if ((module->STA & SDMMC_STA_DATAEND) && (count > 1)) {
		if (SendCommand(module, SDCommand::STOP_TRANSMISSION, 0, SDResponse::SHORT)) {
			return RES_ERROR;
		}
		if (module->RESP1 & 0xFD'FF'E0'08) {
			return RES_ERROR;
		}
	}

	if (module->STA & (SDMMC_STA_DTIMEOUT | SDMMC_STA_DCRCFAIL | SDMMC_STA_RXOVERR)) {
		return RES_ERROR;
	}

	uint32_t timeout = 0xFF'FF'FF'FF;
	while ((module->STA & SDMMC_STA_RXDAVL) && timeout) {
		*tmpBuff = module->FIFO;
		tmpBuff++;
		timeout--;
	}

	ClearFlags(module);

	return RES_OK;
}

/*
==================
WriteBlocks
==================
*/
static DRESULT WriteBlocks(SDIOModule_t *module, const CardInfo &cardInfo, const BYTE* buff, DWORD sector, UINT count) {
	uint32_t address = sector;
	if (cardInfo.type == 0) {	//Standart capacity
		address *= 512;
	}

	module->DCTRL = 0;

	if (count > 1) {
		if (SendCommand(module, SDCommand::WRITE_MULTIPLE_BLOCK, address, SDResponse::SHORT)) {
			return RES_ERROR;
		}
	} else {
		if (SendCommand(module, SDCommand::WRITE_SINGLE_BLOCK, address, SDResponse::SHORT)) {
			return RES_ERROR;
		}
	}

	if (module->RESP1 & 0xFD'FF'E0'08) {
		return RES_ERROR;
	}

	module->DTIMER = 0xFF'FF'FF'FF;
	module->DLEN = count * 512;
	module->DCTRL = (9 << 4) | SDMMC_DCTRL_DTEN;			// 512 bytes to card

	ClearFlags(module);

	uint32_t totalBytes = count * 512, bytesTransferred = 0, restWords = 0;
	const uint32_t *tempBuff = reinterpret_cast<const uint32_t*>(buff);

	// Write block(s) in polling mode
	while (!(module->STA & (SDMMC_STA_TXUNDERR | SDMMC_STA_DCRCFAIL | SDMMC_STA_DTIMEOUT | SDMMC_STA_DBCKEND))) {
		if (module->STA & SDMMC_STA_TXFIFOHE) {			// at least 8 words can be written into the FIFO
			if (unlikely((totalBytes - bytesTransferred) < 32)) {
				restWords = ((totalBytes - bytesTransferred) % 4 == 0) ? ((totalBytes - bytesTransferred) / 4) : ((totalBytes - bytesTransferred) / 4 + 1);

				for (uint32_t i = 0; i < restWords; i++, tempBuff++, bytesTransferred += 4) {
					module->FIFO = *tempBuff;
				}
			} else {
				for (uint32_t i = 0; i < 8U; i++, tempBuff++) {
					module->FIFO = *tempBuff;
				}

				bytesTransferred += 32;
			}
		}
	}

	// Send stop transmission command in case of multiblock write
	if ((module->STA & SDMMC_STA_DATAEND) && (count > 1)) {
		if (SendCommand(module, SDCommand::STOP_TRANSMISSION, 0, SDResponse::SHORT)) {
			return RES_ERROR;
		}
		if (module->RESP1 & 0xFD'FF'E0'08) {
			return RES_ERROR;
		}
	}

	if (module->STA & (SDMMC_STA_DTIMEOUT | SDMMC_STA_DCRCFAIL | SDMMC_STA_TXUNDERR)) {
		return RES_ERROR;
	}

	ClearFlags(module);

	SDState state;
	DRESULT error = GetCardState(module, cardInfo, state);
	
	while (!error && (state == SDState::PROGRAMMING || state == SDState::RECEIVING)) {
		error = GetCardState(module, cardInfo, state);
	}

	return error;
}

/*
==================
Ioctl
==================
*/
static DRESULT Ioctl(SDIOModule_t *module, const CardInfo &cardInfo, BYTE cmd, void* buff) {
	switch (cmd) {
		/* Generic command (Used by FatFs) */
		case CTRL_SYNC:
			return RES_OK;

		case GET_SECTOR_COUNT:
			*reinterpret_cast<DWORD*>(buff) = cardInfo.capacity / 512;
			return RES_OK;

		case GET_SECTOR_SIZE:
			*reinterpret_cast<WORD*>(buff) = 512;
			return RES_OK;

		case GET_BLOCK_SIZE:
		case CTRL_TRIM:
		
		/* Generic command (Not used by FatFs) */
		case CTRL_POWER:
		case CTRL_LOCK:
		case CTRL_EJECT:
		case CTRL_FORMAT:

		/* MMC/SDC specific ioctl command */
		case MMC_GET_TYPE:
		case MMC_GET_CSD:
		case MMC_GET_CID:
		case MMC_GET_OCR:
		case MMC_GET_SDSTAT:
		case ISDIO_READ:
		case ISDIO_WRITE:
		case ISDIO_MRITE:

		default:
			break;
	}

	return RES_ERROR;
}

/*
==================
SDMMC1InitGPIOpins
==================
*/
static void SDMMC1InitGPIOpins() {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOCEN;

	//config for GPIOC8-12
	GPIOC->MODER |= (GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1 | GPIO_MODER_MODER11_1 | GPIO_MODER_MODER12_1);			//alternate function mode
	GPIOC->MODER &= ~(GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER10_0 | GPIO_MODER_MODER11_0 | GPIO_MODER_MODER12_0);			//
	GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR8 | GPIO_OSPEEDER_OSPEEDR9 | GPIO_OSPEEDER_OSPEEDR10 | GPIO_OSPEEDER_OSPEEDR11 | GPIO_OSPEEDER_OSPEEDR12);	//high speed
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPDR8_0 | GPIO_PUPDR_PUPDR9_0 | GPIO_PUPDR_PUPDR10_0 | GPIO_PUPDR_PUPDR11_0 | GPIO_PUPDR_PUPDR12_0);			//pull-up
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR8_1 | GPIO_PUPDR_PUPDR9_1 | GPIO_PUPDR_PUPDR10_1 | GPIO_PUPDR_PUPDR11_1 | GPIO_PUPDR_PUPDR12_1);			//
	GPIOC->AFR[1] |= GPIO_AFRH_AFRH0_3 | GPIO_AFRH_AFRH0_2 | GPIO_AFRH_AFRH1_3 | GPIO_AFRH_AFRH1_2 |										//Select AF12
		GPIO_AFRH_AFRH2_3 | GPIO_AFRH_AFRH2_2 | GPIO_AFRH_AFRH3_3 | GPIO_AFRH_AFRH3_2 | GPIO_AFRH_AFRH4_3 | GPIO_AFRH_AFRH4_2;				//
	GPIOC->AFR[1] &= ~(GPIO_AFRH_AFRH0_1 | GPIO_AFRH_AFRH0_0 | GPIO_AFRH_AFRH1_1 | GPIO_AFRH_AFRH1_0 |										//
		GPIO_AFRH_AFRH2_1 | GPIO_AFRH_AFRH2_0 | GPIO_AFRH_AFRH3_1 | GPIO_AFRH_AFRH3_0 | GPIO_AFRH_AFRH4_1 | GPIO_AFRH_AFRH4_0);				//

																																			//config for GPIOD2
	GPIOD->MODER |= GPIO_MODER_MODER2_1;		//alternate function mode
	GPIOD->MODER &= ~(GPIO_MODER_MODER2_0);		//
	GPIOD->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR2;	//high speed
	GPIOD->PUPDR |= GPIO_PUPDR_PUPDR2_0;		//pull-up
	GPIOD->PUPDR &= ~(GPIO_PUPDR_PUPDR2_1);		//
	GPIOD->AFR[0] |= GPIO_AFRL_AFRL2_3 | GPIO_AFRL_AFRL2_2;		//Select AF12
	GPIOD->AFR[0] &= ~(GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0);	//
}

/*
==================
SDMMC2InitGPIOpins
==================
*/
static void SDMMC2InitGPIOpins() {

}

/*
==================
disk_initialize
==================
*/
DSTATUS disk_initialize(BYTE pdrv) {
	switch (pdrv) {
		case 0:			//bottom sd
			RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;		// clock enable for GPIOD
			GPIOD->MODER &= ~(GPIO_MODER_MODER3);		// PD3 to input - SDMMC1 detect
			GPIOD->PUPDR |= GPIO_PUPDR_PUPDR3_0;		// enable Pull-Up
			GPIOD->PUPDR &= ~(GPIO_PUPDR_PUPDR3_1);		//

			if (!(GPIOD->IDR & GPIO_IDR_IDR_3)) {
				SDMMC1InitGPIOpins();
				RCC->APB2ENR |= RCC_APB2ENR_SDMMC1EN;			// enable clock for SDMMC1 module
				return InitSDCard(SDMMC1, g_bottomCardInfo, true);
			}

			return STA_NODISK | STA_NOINIT;

		case 1:			//top sd
			SDMMC2InitGPIOpins();
			RCC->APB2ENR |= RCC_APB2ENR_SDMMC2EN;
			return InitSDCard(SDMMC2, g_topCardInfo, false);	// this socket don't have card detect pin

		default:
			break;
	}

	return STA_NOINIT;
}

/*
==================
disk_status
==================
*/
DSTATUS disk_status(BYTE pdrv) {
	switch (pdrv) {
		case 0:			//low sd
			return g_bottomCardInfo.initialized ? 0 : STA_NOINIT;

		case 1:			//top sd
			return g_topCardInfo.initialized ? 0 : STA_NOINIT;
			break;

		default:
			break;
	}

	return STA_NOINIT;
}

/*
==================
disk_read
==================
*/
DRESULT disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count) {
	switch (pdrv) {
		case 0:			//low sd
			return ReadBlocks(SDMMC1, g_bottomCardInfo, buff, sector, count);

		case 1:			//top sd
			return ReadBlocks(SDMMC2, g_topCardInfo, buff, sector, count);
			break;

		default:
			break;
	}

	return RES_ERROR;
}

/*
==================
disk_write
==================
*/
DRESULT disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count) {
	switch (pdrv) {
		case 0:			//low sd
			return WriteBlocks(SDMMC1, g_bottomCardInfo, buff, sector, count);

		case 1:			//top sd
			return WriteBlocks(SDMMC2, g_topCardInfo, buff, sector, count);
			break;

		default:
			break;
	}

	return RES_ERROR;
}

/*
==================
disk_ioctl
==================
*/
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void* buff) {
	switch (pdrv) {
		case 0:			//low sd
			return Ioctl(SDMMC1, g_bottomCardInfo, cmd, buff);

		case 1:			//top sd
			return Ioctl(SDMMC2, g_topCardInfo, cmd, buff);
			break;

		default:
			break;
	}

	return RES_ERROR;
}
