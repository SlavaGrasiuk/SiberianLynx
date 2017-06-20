#include <commdef.hpp>

#define WEAK_ISR	[[gnu::weak, gnu::alias("Default_Handler")]]

MANGOFF void Default_Handler();
MANGOFF [[noreturn]] void Reset_Handler();
MANGOFF void __libc_init_array();

namespace core {
	void InitCPU();
	[[noreturn]] void Main();
}

/* CPU interrupt vectors */
MANGOFF {
	WEAK_ISR void NMI_Handler();
	WEAK_ISR void HardFault_Handler();
	WEAK_ISR void MemManage_Handler();
	WEAK_ISR void BusFault_Handler();
	WEAK_ISR void UsageFault_Handler();
	WEAK_ISR void SVC_Handler();
	WEAK_ISR void DebugMon_Handler();
	WEAK_ISR void PendSV_Handler();
	WEAK_ISR void SysTick_Handler();
}

extern unsigned int _estack;		//defined in linker script

using intfunc = void(*)();

[[gnu::used, gnu::section(".isr_vector_cpu")]] static const intfunc g_cpuVectors[] = {
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
	SysTick_Handler
};

static_assert(sizeof g_cpuVectors / sizeof(*g_cpuVectors) == 16, "Invalid size of g_cpuVectors[]!");

/*
==================
Reset_Handler
==================
*/
void Reset_Handler() {
	extern unsigned int _sidata, _sdata, _edata, _sbss, _ebss;		//defined in linker script
	auto pulSrc = &_sidata;

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
_init

	__libc_init_array callback
==================
*/
MANGOFF void _init() {
	core::InitCPU();
}

#pragma region DevVectors

/* STM32F765ZG interrupt vectors */
extern "C" {
	WEAK_ISR void WWDG_IRQHandler();					/* Window WatchDog								*/
	WEAK_ISR void PVD_IRQHandler();						/* PVD through EXTI Line detection				*/
	WEAK_ISR void TAMP_STAMP_IRQHandler();				/* Tamper and TimeStamps through the EXTI line	*/
	WEAK_ISR void RTC_WKUP_IRQHandler();				/* RTC Wakeup through the EXTI line				*/
	WEAK_ISR void FLASH_IRQHandler();					/* FLASH										*/
	WEAK_ISR void RCC_IRQHandler();						/* RCC											*/
	WEAK_ISR void EXTI0_IRQHandler();					/* EXTI Line0									*/
	WEAK_ISR void EXTI1_IRQHandler();					/* EXTI Line1									*/
	WEAK_ISR void EXTI2_IRQHandler();					/* EXTI Line2									*/
	WEAK_ISR void EXTI3_IRQHandler();					/* EXTI Line3									*/
	WEAK_ISR void EXTI4_IRQHandler();					/* EXTI Line4									*/
	WEAK_ISR void DMA1_Stream0_IRQHandler();			/* DMA1 Stream 0								*/
	WEAK_ISR void DMA1_Stream1_IRQHandler();			/* DMA1 Stream 1								*/
	WEAK_ISR void DMA1_Stream2_IRQHandler();			/* DMA1 Stream 2								*/
	WEAK_ISR void DMA1_Stream3_IRQHandler();			/* DMA1 Stream 3								*/
	WEAK_ISR void DMA1_Stream4_IRQHandler();			/* DMA1 Stream 4								*/
	WEAK_ISR void DMA1_Stream5_IRQHandler();			/* DMA1 Stream 5								*/
	WEAK_ISR void DMA1_Stream6_IRQHandler();			/* DMA1 Stream 6								*/
	WEAK_ISR void ADC_IRQHandler();						/* ADC1, ADC2 and ADC3s							*/
	WEAK_ISR void CAN1_TX_IRQHandler();					/* CAN1 TX										*/
	WEAK_ISR void CAN1_RX0_IRQHandler();				/* CAN1 RX0										*/
	WEAK_ISR void CAN1_RX1_IRQHandler();				/* CAN1 RX1										*/
	WEAK_ISR void CAN1_SCE_IRQHandler();				/* CAN1 SCE										*/
	WEAK_ISR void EXTI9_5_IRQHandler();					/* External Line[9:5]s							*/
	WEAK_ISR void TIM1_BRK_TIM9_IRQHandler();			/* TIM1 Break and TIM9							*/
	WEAK_ISR void TIM1_UP_TIM10_IRQHandler();			/* TIM1 Update and TIM10						*/
	WEAK_ISR void TIM1_TRG_COM_TIM11_IRQHandler();		/* TIM1 Trigger and Commutation and TIM11		*/
	WEAK_ISR void TIM1_CC_IRQHandler();					/* TIM1 Capture Compare							*/
	WEAK_ISR void TIM2_IRQHandler();					/* TIM2											*/
	WEAK_ISR void TIM3_IRQHandler();					/* TIM3											*/
	WEAK_ISR void TIM4_IRQHandler();					/* TIM4											*/
	WEAK_ISR void I2C1_EV_IRQHandler();					/* I2C1 Event									*/
	WEAK_ISR void I2C1_ER_IRQHandler();					/* I2C1 Error									*/
	WEAK_ISR void I2C2_EV_IRQHandler();					/* I2C2 Event									*/
	WEAK_ISR void I2C2_ER_IRQHandler();					/* I2C2 Error									*/
	WEAK_ISR void SPI1_IRQHandler();					/* SPI1											*/
	WEAK_ISR void SPI2_IRQHandler();					/* SPI2											*/
	WEAK_ISR void USART1_IRQHandler();					/* USART1										*/
	WEAK_ISR void USART2_IRQHandler();					/* USART2										*/
	WEAK_ISR void USART3_IRQHandler();					/* USART3										*/
	WEAK_ISR void EXTI15_10_IRQHandler();				/* External Line[15:10]s						*/
	WEAK_ISR void RTC_Alarm_IRQHandler();				/* RTC Alarm (A and B) through EXTI Line		*/
	WEAK_ISR void OTG_FS_WKUP_IRQHandler();				/* USB OTG FS Wakeup through EXTI line			*/
	WEAK_ISR void TIM8_BRK_TIM12_IRQHandler();			/* TIM8 Break and TIM12							*/
	WEAK_ISR void TIM8_UP_TIM13_IRQHandler();			/* TIM8 Update and TIM13						*/
	WEAK_ISR void TIM8_TRG_COM_TIM14_IRQHandler();		/* TIM8 Trigger and Commutation and TIM14		*/
	WEAK_ISR void TIM8_CC_IRQHandler();					/* TIM8 Capture Compare							*/
	WEAK_ISR void DMA1_Stream7_IRQHandler();			/* DMA1 Stream7									*/
	WEAK_ISR void FMC_IRQHandler();						/* FMC											*/
	WEAK_ISR void SDMMC1_IRQHandler();					/* SDMMC1										*/
	WEAK_ISR void TIM5_IRQHandler();					/* TIM5											*/
	WEAK_ISR void SPI3_IRQHandler();					/* SPI3											*/
	WEAK_ISR void UART4_IRQHandler();					/* UART4										*/
	WEAK_ISR void UART5_IRQHandler();					/* UART5										*/
	WEAK_ISR void TIM6_DAC_IRQHandler();				/* TIM6 and DAC1&2 underrun errors				*/
	WEAK_ISR void TIM7_IRQHandler();					/* TIM7											*/
	WEAK_ISR void DMA2_Stream0_IRQHandler();			/* DMA2 Stream 0								*/
	WEAK_ISR void DMA2_Stream1_IRQHandler();			/* DMA2 Stream 1								*/
	WEAK_ISR void DMA2_Stream2_IRQHandler();			/* DMA2 Stream 2								*/
	WEAK_ISR void DMA2_Stream3_IRQHandler();			/* DMA2 Stream 3								*/
	WEAK_ISR void DMA2_Stream4_IRQHandler();			/* DMA2 Stream 4								*/
	WEAK_ISR void ETH_IRQHandler();						/* Ethernet										*/
	WEAK_ISR void ETH_WKUP_IRQHandler();				/* Ethernet Wakeup through EXTI line			*/
	WEAK_ISR void CAN2_TX_IRQHandler();					/* CAN2 TX										*/
	WEAK_ISR void CAN2_RX0_IRQHandler();				/* CAN2 RX0										*/
	WEAK_ISR void CAN2_RX1_IRQHandler();				/* CAN2 RX1										*/
	WEAK_ISR void CAN2_SCE_IRQHandler();				/* CAN2 SCE										*/
	WEAK_ISR void OTG_FS_IRQHandler();					/* USB OTG FS									*/
	WEAK_ISR void DMA2_Stream5_IRQHandler();			/* DMA2 Stream 5								*/
	WEAK_ISR void DMA2_Stream6_IRQHandler();			/* DMA2 Stream 6								*/
	WEAK_ISR void DMA2_Stream7_IRQHandler();			/* DMA2 Stream 7								*/
	WEAK_ISR void USART6_IRQHandler();					/* USART6										*/
	WEAK_ISR void I2C3_EV_IRQHandler();					/* I2C3 event									*/
	WEAK_ISR void I2C3_ER_IRQHandler();					/* I2C3 error									*/
	WEAK_ISR void OTG_HS_EP1_OUT_IRQHandler();			/* USB OTG HS End Point 1 Out					*/
	WEAK_ISR void OTG_HS_EP1_IN_IRQHandler();			/* USB OTG HS End Point 1 In					*/
	WEAK_ISR void OTG_HS_WKUP_IRQHandler();				/* USB OTG HS Wakeup through EXTI				*/
	WEAK_ISR void OTG_HS_IRQHandler();					/* USB OTG HS									*/
	WEAK_ISR void DCMI_IRQHandler();					/* DCMI											*/
	WEAK_ISR void RNG_IRQHandler();						/* RNG											*/
	WEAK_ISR void FPU_IRQHandler();						/* FPU											*/
	WEAK_ISR void UART7_IRQHandler();					/* UART7										*/
	WEAK_ISR void UART8_IRQHandler();					/* UART8										*/
	WEAK_ISR void SPI4_IRQHandler();					/* SPI4											*/
	WEAK_ISR void SPI5_IRQHandler();					/* SPI5											*/
	WEAK_ISR void SPI6_IRQHandler();					/* SPI6											*/
	WEAK_ISR void SAI1_IRQHandler();					/* SAI1											*/
	WEAK_ISR void DMA2D_IRQHandler();					/* DMA2D										*/
	WEAK_ISR void SAI2_IRQHandler();					/* SAI2											*/
	WEAK_ISR void QUADSPI_IRQHandler();					/* QUADSPI										*/
	WEAK_ISR void LPTIM1_IRQHandler();					/* LPTIM1										*/
	WEAK_ISR void CEC_IRQHandler();						/* HDMI_CEC										*/
	WEAK_ISR void I2C4_EV_IRQHandler();					/* I2C4 Event									*/
	WEAK_ISR void I2C4_ER_IRQHandler();					/* I2C4 Error									*/
	WEAK_ISR void SPDIF_RX_IRQHandler();				/* SPDIF_RX										*/
	WEAK_ISR void DFSDM1_FLT0_IRQHandler();				/* DFSDM1 Filter 0 global Interrupt				*/
	WEAK_ISR void DFSDM1_FLT1_IRQHandler();				/* DFSDM1 Filter 1 global Interrupt				*/
	WEAK_ISR void DFSDM1_FLT2_IRQHandler();				/* DFSDM1 Filter 2 global Interrupt				*/
	WEAK_ISR void DFSDM1_FLT3_IRQHandler();				/* DFSDM1 Filter 3 global Interrupt				*/
	WEAK_ISR void SDMMC2_IRQHandler();					/* SDMMC2										*/
	WEAK_ISR void CAN3_TX_IRQHandler();					/* CAN3 TX										*/
	WEAK_ISR void CAN3_RX0_IRQHandler();				/* CAN3 RX0										*/
	WEAK_ISR void CAN3_RX1_IRQHandler();				/* CAN3 RX1										*/
	WEAK_ISR void CAN3_SCE_IRQHandler();				/* CAN3 SCE										*/
	WEAK_ISR void MDIOS_IRQHandler();					/* MDIOS										*/
}

[[gnu::used, gnu::section(".isr_vector_dev")]] static const intfunc g_devVectors[] = {
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

static_assert(sizeof g_devVectors / sizeof(*g_devVectors) == 110, "Invalid size of g_devVectors[]!");

#pragma endregion

/*
==================
Default_Handler
==================
*/
void Default_Handler() {
	EXCEPT_HNDL()
}
