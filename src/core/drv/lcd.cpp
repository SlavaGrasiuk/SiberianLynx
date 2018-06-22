#include "../procsdef.hpp"
#include "../commdef.hpp"
#include "lcd.hpp"
#include <stm32f7xx.h>
#include <stm32f7xx_hal.h>
#include <scmRTOS.h>
#include <thread>


constexpr int LCD_RS_ADRESS_BIT = 18;		//A18 used as RS signal

constexpr unsigned int FMC_BANK1_BASE = 0x60'00'00'00;

static core::BackLightCtrl g_backLightCtrl;

static OS::TEventFlag g_vsyncPulse;			//For software emulation of the adaptive vsync

static volatile uint16_t * const g_lcdCommand = reinterpret_cast<uint16_t*>(FMC_BANK1_BASE);
static volatile uint16_t * const g_lcdData = reinterpret_cast<uint16_t*>(FMC_BANK1_BASE | (1 << (LCD_RS_ADRESS_BIT + 1)));

static uint16_t g_backBuffer[320 * 240] alignas(32);		//align for cache cleaning


void DelayMs(const unsigned int ms);


/*
==================
TIM1_UP_TIM10_IRQHandler

	Vsync timer interrupt
==================
*/
extern "C" void TIM1_UP_TIM10_IRQHandler() {
	if (likely(TIM10->SR & TIM_SR_UIF)) {
		TIM10->SR &= ~(TIM_SR_UIF);
		g_vsyncPulse.signal_isr();
	} else {
		EXCEPT_HNDL()
	}
}

/*
==================
LcdSendCommand
==================
*/
static inline void LcdSendCommand(const uint16_t reg, const uint16_t val) {
	*g_lcdCommand = reg;
	*g_lcdData = val;
}

/*
==================
core::InitMPU

	Disable caching of the FMC bank 1. This bank used for communication with LCD.
==================
*/
void core::InitMPU() {
	MPU_Region_InitTypeDef MPU_InitStruct;

	/* Disable the MPU */
	HAL_MPU_Disable();
	
	/* Configure the MPU attributes for 16Mb NOR Flash*/
	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress = FMC_BANK1_BASE;
	MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER1;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.SubRegionDisable = 0x00;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);

	/* Enable the MPU */
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/*
==================
core::InitLcdInterface
==================
*/
void core::InitLcdInterface() {
	//reset pin
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	MODIFY_REG(GPIOB->MODER, GPIO_MODER_MODER9, (0b01 << GPIO_MODER_MODER9_Pos));	//output mode
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT_9);	//push-pull
	MODIFY_REG(GPIOB->OSPEEDR, GPIO_OSPEEDER_OSPEEDR9, (0b10 << GPIO_OSPEEDER_OSPEEDR9_Pos));	//high speed, on lower speeds doesn't work for some reason
	MODIFY_REG(GPIOB->PUPDR, GPIO_PUPDR_PUPDR9, 0);			//No pull-up, pull-down

	//PD4, PD5, PD13
	MODIFY_REG(GPIOD->AFR[0], (GPIO_AFRL_AFRL4 | GPIO_AFRL_AFRL5), ((12 << GPIO_AFRL_AFRL4_Pos) | (12 << GPIO_AFRL_AFRL5_Pos)));
	MODIFY_REG(GPIOD->AFR[1], GPIO_AFRH_AFRH5, (12 << GPIO_AFRH_AFRH5_Pos));
	/* Configure PDx pins in Alternate function mode */
	MODIFY_REG(GPIOD->MODER, (GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER13), ((0b10 << GPIO_MODER_MODER4_Pos) | (0b10 << GPIO_MODER_MODER5_Pos) | (0b10 << GPIO_MODER_MODER13_Pos)));
	/* Configure PDx pins to very high speed */
	MODIFY_REG(GPIOD->OSPEEDR, (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR13), ((0b11 << GPIO_OSPEEDER_OSPEEDR4_Pos) | (0b11 << GPIO_OSPEEDER_OSPEEDR5_Pos) | (0b11 << GPIO_OSPEEDER_OSPEEDR13_Pos)));
	/* Configure PDx pins Output type to push-pull */
	MODIFY_REG(GPIOD->OTYPER, (GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_13), 0);
	/* No pull-up and no pull-down for PDx pins */
	MODIFY_REG(GPIOD->PUPDR, (GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR13), 0);

	//PC7
	MODIFY_REG(GPIOC->AFR[0], GPIO_AFRL_AFRL7, (9 << GPIO_AFRL_AFRL7_Pos));
	/* Configure PDx pins in Alternate function mode */
	MODIFY_REG(GPIOC->MODER, GPIO_MODER_MODER7, (0b10 << GPIO_MODER_MODER7_Pos));
	/* Configure PDx pins to very high speed */
	MODIFY_REG(GPIOC->OSPEEDR, GPIO_OSPEEDER_OSPEEDR7, (0b11 << GPIO_OSPEEDER_OSPEEDR7_Pos));
	/* Configure PDx pins Output type to push-pull */
	MODIFY_REG(GPIOC->OTYPER, GPIO_OTYPER_OT_7, 0);
	/* No pull-up and no pull-down for PDx pins */
	MODIFY_REG(GPIOC->PUPDR, GPIO_PUPDR_PUPDR7, 0);

	//data bus already initialized during sdram initialization

	FMC_NORSRAM_TimingTypeDef sram_timing;
	SRAM_HandleTypeDef hsram;

	hsram.Instance  = FMC_NORSRAM_DEVICE;
	hsram.Extended  = FMC_NORSRAM_EXTENDED_DEVICE;

	/* Set parameters for LCD access (FMC_NORSRAM_BANK1) */
	hsram.Init.NSBank             = FMC_NORSRAM_BANK1;
	hsram.Init.DataAddressMux     = FMC_DATA_ADDRESS_MUX_DISABLE;
	hsram.Init.MemoryType         = FMC_MEMORY_TYPE_NOR;
	hsram.Init.MemoryDataWidth    = FMC_NORSRAM_MEM_BUS_WIDTH_16;
	hsram.Init.BurstAccessMode    = FMC_BURST_ACCESS_MODE_DISABLE;
	hsram.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram.Init.WaitSignalActive   = FMC_WAIT_TIMING_BEFORE_WS;
	hsram.Init.WriteOperation     = FMC_WRITE_OPERATION_ENABLE;
	hsram.Init.WaitSignal         = FMC_WAIT_SIGNAL_DISABLE;
	hsram.Init.ExtendedMode       = FMC_EXTENDED_MODE_DISABLE;
	hsram.Init.AsynchronousWait   = FMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram.Init.WriteBurst         = FMC_WRITE_BURST_DISABLE;
	hsram.Init.WriteFifo          = FMC_WRITE_FIFO_DISABLE;
	hsram.Init.PageSize           = FMC_PAGE_SIZE_NONE;
	hsram.Init.ContinuousClock    = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;

	/* PSRAM device configuration */
	/* Timing configuration derived from system clock (up to 216Mhz)
		for 108Mhz as PSRAM clock frequency */
	sram_timing.AddressSetupTime      = 2;
	sram_timing.AddressHoldTime       = 0;
	sram_timing.DataSetupTime         = 5;
	sram_timing.BusTurnAroundDuration = 0;
	sram_timing.CLKDivision           = 1;
	sram_timing.DataLatency           = 2;
	sram_timing.AccessMode            = FMC_ACCESS_MODE_B;

	/* Initialize the FMC controller for LCD (FMC_NORSRAM_BANK1) */
	HAL_SRAM_Init(&hsram, &sram_timing, &sram_timing);
}

/*
==================
core::InitLcd
==================
*/
void core::InitLcd() {
	DelayMs(1);

	//reset display
	GPIOB->BSRR = GPIO_BSRR_BR_9;		//reset low
	DelayMs(5);
	GPIOB->BSRR = GPIO_BSRR_BS_9;		//reset high
	DelayMs(10);
	GPIOB->BSRR = GPIO_BSRR_BR_9;		//reset low
	DelayMs(20);
	GPIOB->BSRR = GPIO_BSRR_BS_9;		//reset high
	DelayMs(10);
	
	*g_lcdCommand = 100;
	volatile uint16_t val = *g_lcdData;

	//init display
	LcdSendCommand(0x00'07, 0x00'21);
	LcdSendCommand(0x00'00, 0x00'01);
	LcdSendCommand(0x00'07, 0x00'23);
	LcdSendCommand(0x00'10, 0x00'00);
	DelayMs(30);
	LcdSendCommand(0x00'07, 0x00'33);
	LcdSendCommand(0x00'11, 0x68'38);
	LcdSendCommand(0x00'02, 0x06'00);

	// power control 1 
	// DCT3 DCT2 DCT1 DCT0 BT2 BT1 BT0 0 DC3 DC2 DC1 DC0 AP2 AP1 AP0 0 
	// 1     0    1    0    1   0   0  0  1   0   1   0   0   1   0  0 
	// DCT[3:0] fosc/4 BT[2:0]  DC{3:0] fosc/4 
	LcdSendCommand(0x0003, 0x0804);//0xA8A4 
	LcdSendCommand(0x000C, 0x0000);// 
	LcdSendCommand(0x000D, 0x0808);// 0x080C --> 0x0808 
	// power control 4 
	// 0 0 VCOMG VDV4 VDV3 VDV2 VDV1 VDV0 0 0 0 0 0 0 0 0 
	// 0 0   1    0    1    0    1    1   0 0 0 0 0 0 0 0 
	LcdSendCommand(0x000E, 0x2900);
	LcdSendCommand(0x001E, 0x00B8);
	// Driver output control 
	// 0 RL REV CAD BGR SM TB MUX8 MUX7 MUX6 MUX5 MUX4 MUX3 MUX2 MUX1 MUX0 
	// 0 0  1    0   1  0  1  1     0    0    1    1    1    1    1    1 
	// 14.RL  = Output shift direction of Source driver (1 = S0->S719; 0 = S719->S0) 
	// 13.REV = Grayscale mode (1 = normal; 0 = inverted) 
	// 11.BGR = Components order(1 = BGR; 0 = RGB) 
	// 09.TB  = Output shift direction of Gate driver (1 = G0->G319; 0 = G319->G0) 
	// 08..00.MUX = Number of lines (0..319) 
	LcdSendCommand(0x0001, (0 << 14) | (1 << 13) | (1 << 11) | (0 << 9) | (1 << 8) | (1 << 5) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0));
	LcdSendCommand(0x0010, 0x0000);
	LcdSendCommand(0x0005, 0x0000);
	LcdSendCommand(0x0006, 0x0000);
	LcdSendCommand(0x0016, 0xEF1C);
	LcdSendCommand(0x0017, 0x0003);
	LcdSendCommand(0x0007, 0x0233);
	LcdSendCommand(0x000B, 0x0000 | (3 << 6));
	LcdSendCommand(0x000F, 0x0000);
	LcdSendCommand(0x0041, 0x0000);
	LcdSendCommand(0x0042, 0x0000);
	LcdSendCommand(0x0048, 0x0000);
	LcdSendCommand(0x0049, 0x013F);
	LcdSendCommand(0x004A, 0x0000);
	LcdSendCommand(0x004B, 0x0000);
	LcdSendCommand(0x0044, 0xEF00);
	LcdSendCommand(0x0045, 0x0000);
	LcdSendCommand(0x0046, 0x013F);

	// Gamma control 
/*	LcdSendCommand(0x0030, 0x0707);
	LcdSendCommand(0x0031, 0x0204);
	LcdSendCommand(0x0032, 0x0204);
	LcdSendCommand(0x0033, 0x0502);
	LcdSendCommand(0x0034, 0x0507);
	LcdSendCommand(0x0035, 0x0204);
	LcdSendCommand(0x0036, 0x0204);
	LcdSendCommand(0x0037, 0x0502);
	LcdSendCommand(0x003A, 0x0302);
	LcdSendCommand(0x003B, 0x0302);*/
	// Gamma control end 
	LcdSendCommand(0x0023, 0x0000);
	LcdSendCommand(0x0024, 0x0000);
	LcdSendCommand(0x0025, 0x8000);   // 65hz 
	LcdSendCommand(0x004f, 0);
	LcdSendCommand(0x004e, 0);
	*g_lcdCommand = 0x22;

	//software vsync timer init
	RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;		//enable timer clock
	TIM10->DIER |= TIM_DIER_UIE;				//enable update interrupt
	TIM10->PSC = SYSTICKFREQ / 60'000 - 1;		//timer clock 60'000 Hz
	TIM10->ARR = 999;							//timer counts from 0 to ARR, so 60'000 / 1'000 = 60 Hz
	TIM10->CNT = 0;
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

	MODIFY_REG(GPIOA->MODER, GPIO_MODER_MODER15, (0b01 << GPIO_MODER_MODER15_Pos));	//output mode
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_15);	//push-pull
	MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR15, (0b10 << GPIO_OSPEEDER_OSPEEDR15_Pos));	//high speed
	MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR15, 0);			//No pull-up, pull-down
}

/*
==================
core::LcdRunTimer
==================
*/
void core::LcdRunTimer() {
	TIM10->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;	//enable count
}

/*
==================
core::LcdSync
==================
*/
void core::LcdSync() {
	SCB_CleanDCache_by_Addr(reinterpret_cast<uint32_t*>(g_backBuffer), sizeof g_backBuffer);	//sync cache with internal RAM
	g_vsyncPulse.wait();	//wait for vsync

	for (auto &pixel : g_backBuffer) {
		*g_lcdData = pixel;
	}
}

/*
==================
core::FillLcdRandom
==================
*/
void core::FillLcdRandom() {
	RCC->AHB2ENR |= RCC_AHB2ENR_RNGEN;
	RNG->CR |= RNG_CR_RNGEN;

	for (uint32_t* data = reinterpret_cast<uint32_t*>(g_backBuffer); data < reinterpret_cast<uint32_t*>(uint32_t(g_backBuffer) + sizeof g_backBuffer); data++) {
		while (!(RNG->SR & RNG_SR_DRDY));
		*data = RNG->DR;
	}

	RNG->CR &= ~(RNG_CR_RNGEN);
	RCC->AHB2ENR &= ~(RCC_AHB2ENR_RNGEN);
}

/*
==================
core::LcdFillColor
==================
*/
void core::LcdFillColor(const uint16_t color) {
	for (auto &pixel : g_backBuffer) {
		pixel = color;
	}
}

/*
==================
core::InitLcdBacklight
==================
*/
void core::InitLcdBacklight() {
	GPIOA->MODER |= GPIO_MODER_MODER2_0;	//output mode
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT_2);	//push-pull
	MODIFY_REG(GPIOA->OSPEEDR, GPIO_OSPEEDER_OSPEEDR2, 0);	//low speed
	MODIFY_REG(GPIOA->PUPDR, GPIO_PUPDR_PUPDR2, 0);			//No pull-up, pull-down

	GPIOA->BSRR = GPIO_BSRR_BR_2;		//disable backlight
}

/*
==================
core::BackLightCtrl::exec
==================
*/
template<> void core::BackLightCtrl::exec() {
	OS::sleep();

	while (1) {

	}
}
