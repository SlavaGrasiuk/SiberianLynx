#include "../commdef.hpp"

#define WEAK_ALIAS	[[gnu::weak, gnu::alias("Default_Handler")]]

extern "C" void Default_Handler();

extern "C" {
	/* CPU interrupt vectors */
	WEAK_ALIAS void NMI_Handler();
	WEAK_ALIAS void HardFault_Handler();
	WEAK_ALIAS void MemManage_Handler();
	WEAK_ALIAS void BusFault_Handler();
	WEAK_ALIAS void UsageFault_Handler();
	WEAK_ALIAS void SVC_Handler();
	WEAK_ALIAS void DebugMon_Handler();
	WEAK_ALIAS void PendSV_Handler();
	WEAK_ALIAS void SysTick_Handler();

	/* STM32F765ZG interrupt vectors */
	WEAK_ALIAS void WWDG_IRQHandler();					/* Window WatchDog								*/
	WEAK_ALIAS void PVD_IRQHandler();					/* PVD through EXTI Line detection				*/
	WEAK_ALIAS void TAMP_STAMP_IRQHandler();			/* Tamper and TimeStamps through the EXTI line	*/
	WEAK_ALIAS void RTC_WKUP_IRQHandler();				/* RTC Wakeup through the EXTI line				*/
	WEAK_ALIAS void FLASH_IRQHandler();					/* FLASH										*/
	WEAK_ALIAS void RCC_IRQHandler();					/* RCC											*/
	WEAK_ALIAS void EXTI0_IRQHandler();					/* EXTI Line0									*/
	WEAK_ALIAS void EXTI1_IRQHandler();					/* EXTI Line1									*/
	WEAK_ALIAS void EXTI2_IRQHandler();					/* EXTI Line2									*/
	WEAK_ALIAS void EXTI3_IRQHandler();					/* EXTI Line3									*/
	WEAK_ALIAS void EXTI4_IRQHandler();					/* EXTI Line4									*/
	WEAK_ALIAS void DMA1_Stream0_IRQHandler();			/* DMA1 Stream 0								*/
	WEAK_ALIAS void DMA1_Stream1_IRQHandler();			/* DMA1 Stream 1								*/
	WEAK_ALIAS void DMA1_Stream2_IRQHandler();			/* DMA1 Stream 2								*/
	WEAK_ALIAS void DMA1_Stream3_IRQHandler();			/* DMA1 Stream 3								*/
	WEAK_ALIAS void DMA1_Stream4_IRQHandler();			/* DMA1 Stream 4								*/
	WEAK_ALIAS void DMA1_Stream5_IRQHandler();			/* DMA1 Stream 5								*/
	WEAK_ALIAS void DMA1_Stream6_IRQHandler();			/* DMA1 Stream 6								*/
	WEAK_ALIAS void ADC_IRQHandler();					/* ADC1, ADC2 and ADC3s							*/
	WEAK_ALIAS void CAN1_TX_IRQHandler();				/* CAN1 TX										*/
	WEAK_ALIAS void CAN1_RX0_IRQHandler();				/* CAN1 RX0										*/
	WEAK_ALIAS void CAN1_RX1_IRQHandler();				/* CAN1 RX1										*/
	WEAK_ALIAS void CAN1_SCE_IRQHandler();				/* CAN1 SCE										*/
	WEAK_ALIAS void EXTI9_5_IRQHandler();				/* External Line[9:5]s							*/
	WEAK_ALIAS void TIM1_BRK_TIM9_IRQHandler();			/* TIM1 Break and TIM9							*/
	WEAK_ALIAS void TIM1_UP_TIM10_IRQHandler();			/* TIM1 Update and TIM10						*/
	WEAK_ALIAS void TIM1_TRG_COM_TIM11_IRQHandler();	/* TIM1 Trigger and Commutation and TIM11		*/
	WEAK_ALIAS void TIM1_CC_IRQHandler();				/* TIM1 Capture Compare							*/
	WEAK_ALIAS void TIM2_IRQHandler();					/* TIM2											*/
	WEAK_ALIAS void TIM3_IRQHandler();					/* TIM3											*/
	WEAK_ALIAS void TIM4_IRQHandler();					/* TIM4											*/
	WEAK_ALIAS void I2C1_EV_IRQHandler();				/* I2C1 Event									*/
	WEAK_ALIAS void I2C1_ER_IRQHandler();				/* I2C1 Error									*/
	WEAK_ALIAS void I2C2_EV_IRQHandler();				/* I2C2 Event									*/
	WEAK_ALIAS void I2C2_ER_IRQHandler();				/* I2C2 Error									*/
	WEAK_ALIAS void SPI1_IRQHandler();					/* SPI1											*/
	WEAK_ALIAS void SPI2_IRQHandler();					/* SPI2											*/
	WEAK_ALIAS void USART1_IRQHandler();				/* USART1										*/
	WEAK_ALIAS void USART2_IRQHandler();				/* USART2										*/
	WEAK_ALIAS void USART3_IRQHandler();				/* USART3										*/
	WEAK_ALIAS void EXTI15_10_IRQHandler();				/* External Line[15:10]s						*/
	WEAK_ALIAS void RTC_Alarm_IRQHandler();				/* RTC Alarm (A and B) through EXTI Line		*/
	WEAK_ALIAS void OTG_FS_WKUP_IRQHandler();			/* USB OTG FS Wakeup through EXTI line			*/
	WEAK_ALIAS void TIM8_BRK_TIM12_IRQHandler();		/* TIM8 Break and TIM12							*/
	WEAK_ALIAS void TIM8_UP_TIM13_IRQHandler();			/* TIM8 Update and TIM13						*/
	WEAK_ALIAS void TIM8_TRG_COM_TIM14_IRQHandler();	/* TIM8 Trigger and Commutation and TIM14		*/
	WEAK_ALIAS void TIM8_CC_IRQHandler();				/* TIM8 Capture Compare							*/
	WEAK_ALIAS void DMA1_Stream7_IRQHandler();			/* DMA1 Stream7									*/
	WEAK_ALIAS void FMC_IRQHandler();					/* FMC											*/
	WEAK_ALIAS void SDMMC1_IRQHandler();				/* SDMMC1										*/
	WEAK_ALIAS void TIM5_IRQHandler();					/* TIM5											*/
	WEAK_ALIAS void SPI3_IRQHandler();					/* SPI3											*/
	WEAK_ALIAS void UART4_IRQHandler();					/* UART4										*/
	WEAK_ALIAS void UART5_IRQHandler();					/* UART5										*/
	WEAK_ALIAS void TIM6_DAC_IRQHandler();				/* TIM6 and DAC1&2 underrun errors				*/
	WEAK_ALIAS void TIM7_IRQHandler();					/* TIM7											*/
	WEAK_ALIAS void DMA2_Stream0_IRQHandler();			/* DMA2 Stream 0								*/
	WEAK_ALIAS void DMA2_Stream1_IRQHandler();			/* DMA2 Stream 1								*/
	WEAK_ALIAS void DMA2_Stream2_IRQHandler();			/* DMA2 Stream 2								*/
	WEAK_ALIAS void DMA2_Stream3_IRQHandler();			/* DMA2 Stream 3								*/
	WEAK_ALIAS void DMA2_Stream4_IRQHandler();			/* DMA2 Stream 4								*/
	WEAK_ALIAS void ETH_IRQHandler();					/* Ethernet										*/
	WEAK_ALIAS void ETH_WKUP_IRQHandler();				/* Ethernet Wakeup through EXTI line			*/
	WEAK_ALIAS void CAN2_TX_IRQHandler();				/* CAN2 TX										*/
	WEAK_ALIAS void CAN2_RX0_IRQHandler();				/* CAN2 RX0										*/
	WEAK_ALIAS void CAN2_RX1_IRQHandler();				/* CAN2 RX1										*/
	WEAK_ALIAS void CAN2_SCE_IRQHandler();				/* CAN2 SCE										*/
	WEAK_ALIAS void OTG_FS_IRQHandler();				/* USB OTG FS									*/
	WEAK_ALIAS void DMA2_Stream5_IRQHandler();			/* DMA2 Stream 5								*/
	WEAK_ALIAS void DMA2_Stream6_IRQHandler();			/* DMA2 Stream 6								*/
	WEAK_ALIAS void DMA2_Stream7_IRQHandler();			/* DMA2 Stream 7								*/
	WEAK_ALIAS void USART6_IRQHandler();				/* USART6										*/
	WEAK_ALIAS void I2C3_EV_IRQHandler();				/* I2C3 event									*/
	WEAK_ALIAS void I2C3_ER_IRQHandler();				/* I2C3 error									*/
	WEAK_ALIAS void OTG_HS_EP1_OUT_IRQHandler();		/* USB OTG HS End Point 1 Out					*/
	WEAK_ALIAS void OTG_HS_EP1_IN_IRQHandler();			/* USB OTG HS End Point 1 In					*/
	WEAK_ALIAS void OTG_HS_WKUP_IRQHandler();			/* USB OTG HS Wakeup through EXTI				*/
	WEAK_ALIAS void OTG_HS_IRQHandler();				/* USB OTG HS									*/
	WEAK_ALIAS void DCMI_IRQHandler();					/* DCMI											*/
	WEAK_ALIAS void RNG_IRQHandler();					/* RNG											*/
	WEAK_ALIAS void FPU_IRQHandler();					/* FPU											*/
	WEAK_ALIAS void UART7_IRQHandler();					/* UART7										*/
	WEAK_ALIAS void UART8_IRQHandler();					/* UART8										*/
	WEAK_ALIAS void SPI4_IRQHandler();					/* SPI4											*/
	WEAK_ALIAS void SPI5_IRQHandler();					/* SPI5											*/
	WEAK_ALIAS void SPI6_IRQHandler();					/* SPI6											*/
	WEAK_ALIAS void SAI1_IRQHandler();					/* SAI1											*/
	WEAK_ALIAS void DMA2D_IRQHandler();					/* DMA2D										*/
	WEAK_ALIAS void SAI2_IRQHandler();					/* SAI2											*/
	WEAK_ALIAS void QUADSPI_IRQHandler();				/* QUADSPI										*/
	WEAK_ALIAS void LPTIM1_IRQHandler();				/* LPTIM1										*/
	WEAK_ALIAS void CEC_IRQHandler();					/* HDMI_CEC										*/
	WEAK_ALIAS void I2C4_EV_IRQHandler();				/* I2C4 Event									*/
	WEAK_ALIAS void I2C4_ER_IRQHandler();				/* I2C4 Error									*/
	WEAK_ALIAS void SPDIF_RX_IRQHandler();				/* SPDIF_RX										*/
	WEAK_ALIAS void DFSDM1_FLT0_IRQHandler();			/* DFSDM1 Filter 0 global Interrupt				*/
	WEAK_ALIAS void DFSDM1_FLT1_IRQHandler();			/* DFSDM1 Filter 1 global Interrupt				*/
	WEAK_ALIAS void DFSDM1_FLT2_IRQHandler();			/* DFSDM1 Filter 2 global Interrupt				*/
	WEAK_ALIAS void DFSDM1_FLT3_IRQHandler();			/* DFSDM1 Filter 3 global Interrupt				*/
	WEAK_ALIAS void SDMMC2_IRQHandler();				/* SDMMC2										*/
	WEAK_ALIAS void CAN3_TX_IRQHandler();				/* CAN3 TX										*/
	WEAK_ALIAS void CAN3_RX0_IRQHandler();				/* CAN3 RX0										*/
	WEAK_ALIAS void CAN3_RX1_IRQHandler();				/* CAN3 RX1										*/
	WEAK_ALIAS void CAN3_SCE_IRQHandler();				/* CAN3 SCE										*/
	WEAK_ALIAS void MDIOS_IRQHandler();					/* MDIOS										*/
}

extern "C" [[noreturn]] void Reset_Handler();		//used in linker script as entry point

extern unsigned int _estack;						//end of internal RAM, defined in linker script

using intfunc = void(*)();

[[gnu::used, gnu::section(".isr_vector")]] static const intfunc g_vectors[] = {
	reinterpret_cast<intfunc>(&_estack),
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	SVC_Handler,
	DebugMon_Handler,
	nullptr,
	PendSV_Handler,
	SysTick_Handler,

	WWDG_IRQHandler,
	PVD_IRQHandler,
	TAMP_STAMP_IRQHandler,
	RTC_WKUP_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Stream0_IRQHandler,
	DMA1_Stream1_IRQHandler,
	DMA1_Stream2_IRQHandler,
	DMA1_Stream3_IRQHandler,
	DMA1_Stream4_IRQHandler,
	DMA1_Stream5_IRQHandler,
	DMA1_Stream6_IRQHandler,
	ADC_IRQHandler,
	CAN1_TX_IRQHandler,
	CAN1_RX0_IRQHandler,
	CAN1_RX1_IRQHandler,
	CAN1_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_TIM9_IRQHandler,
	TIM1_UP_TIM10_IRQHandler,
	TIM1_TRG_COM_TIM11_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM4_IRQHandler,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	I2C2_EV_IRQHandler,
	I2C2_ER_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	USART3_IRQHandler,
	EXTI15_10_IRQHandler,
	RTC_Alarm_IRQHandler,
	OTG_FS_WKUP_IRQHandler,
	TIM8_BRK_TIM12_IRQHandler,
	TIM8_UP_TIM13_IRQHandler,
	TIM8_TRG_COM_TIM14_IRQHandler,
	TIM8_CC_IRQHandler,
	DMA1_Stream7_IRQHandler,
	FMC_IRQHandler,
	SDMMC1_IRQHandler,
	TIM5_IRQHandler,
	SPI3_IRQHandler,
	UART4_IRQHandler,
	UART5_IRQHandler,
	TIM6_DAC_IRQHandler,
	TIM7_IRQHandler,
	DMA2_Stream0_IRQHandler,
	DMA2_Stream1_IRQHandler,
	DMA2_Stream2_IRQHandler,
	DMA2_Stream3_IRQHandler,
	DMA2_Stream4_IRQHandler,
	ETH_IRQHandler,
	ETH_WKUP_IRQHandler,
	CAN2_TX_IRQHandler,
	CAN2_RX0_IRQHandler,
	CAN2_RX1_IRQHandler,
	CAN2_SCE_IRQHandler,
	OTG_FS_IRQHandler,
	DMA2_Stream5_IRQHandler,
	DMA2_Stream6_IRQHandler,
	DMA2_Stream7_IRQHandler,
	USART6_IRQHandler,
	I2C3_EV_IRQHandler,
	I2C3_ER_IRQHandler,
	OTG_HS_EP1_OUT_IRQHandler,
	OTG_HS_EP1_IN_IRQHandler,
	OTG_HS_WKUP_IRQHandler,
	OTG_HS_IRQHandler,
	DCMI_IRQHandler,
	nullptr,
	RNG_IRQHandler,
	FPU_IRQHandler,
	UART7_IRQHandler,
	UART8_IRQHandler,
	SPI4_IRQHandler,
	SPI5_IRQHandler,
	SPI6_IRQHandler,
	SAI1_IRQHandler,
	nullptr,
	nullptr,
	DMA2D_IRQHandler,
	SAI2_IRQHandler,
	QUADSPI_IRQHandler,
	LPTIM1_IRQHandler,
	CEC_IRQHandler,
	I2C4_EV_IRQHandler,
	I2C4_ER_IRQHandler,
	SPDIF_RX_IRQHandler,
	nullptr,
	DFSDM1_FLT0_IRQHandler,
	DFSDM1_FLT1_IRQHandler,
	DFSDM1_FLT2_IRQHandler,
	DFSDM1_FLT3_IRQHandler,
	SDMMC2_IRQHandler,
	CAN3_TX_IRQHandler,
	CAN3_RX0_IRQHandler,
	CAN3_RX1_IRQHandler,
	CAN3_SCE_IRQHandler,
	nullptr,
	MDIOS_IRQHandler
};

static_assert(sizeof g_vectors / sizeof(*g_vectors) == 16 + 110, "Invalid size of g_vectors[]!");	// cortex-m7 + device vectors

extern "C" void __libc_init_array();

namespace core {
	void InitSDRAM();
	[[noreturn]] void Main();
}

/*
==================
Reset_Handler
==================
*/
void Reset_Handler() {
	extern unsigned int _sidata, _sdata, _edata, _sbss, _ebss;		//defined in linker script
	auto pulSrc = &_sidata;

	core::InitSDRAM();		//first action after power up - initialize onboard SDRAM

	/* Copy .data section */
	for (auto pulDest = &_sdata; pulDest < &_edata; ) {
		*(pulDest++) = *(pulSrc++);
	}

	/* Clear .bss section */
	for (auto pulDest = &_sbss; pulDest < &_ebss; ) {
		*(pulDest++) = 0;
	}

	/* Call global constructors from .init_array and .preinit_array sections */
	__libc_init_array();

	core::Main();
}

/*
==================
Default_Handler
==================
*/
void Default_Handler() {
	EXCEPT_HNDL()
}
