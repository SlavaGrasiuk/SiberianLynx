/* STM32F765ZG interrupt vectors */
extern "C" {
	void WWDG_IRQHandler();						/* Window WatchDog								*/
	void PVD_IRQHandler();						/* PVD through EXTI Line detection				*/
	void TAMP_STAMP_IRQHandler();				/* Tamper and TimeStamps through the EXTI line	*/
	void RTC_WKUP_IRQHandler();					/* RTC Wakeup through the EXTI line				*/
	void FLASH_IRQHandler();					/* FLASH										*/
	void RCC_IRQHandler();						/* RCC											*/
	void EXTI0_IRQHandler();					/* EXTI Line0									*/
	void EXTI1_IRQHandler();					/* EXTI Line1									*/
	void EXTI2_IRQHandler();					/* EXTI Line2									*/
	void EXTI3_IRQHandler();					/* EXTI Line3									*/
	void EXTI4_IRQHandler();					/* EXTI Line4									*/
	void DMA1_Stream0_IRQHandler();				/* DMA1 Stream 0								*/
	void DMA1_Stream1_IRQHandler();				/* DMA1 Stream 1								*/
	void DMA1_Stream2_IRQHandler();				/* DMA1 Stream 2								*/
	void DMA1_Stream3_IRQHandler();				/* DMA1 Stream 3								*/
	void DMA1_Stream4_IRQHandler();				/* DMA1 Stream 4								*/
	void DMA1_Stream5_IRQHandler();				/* DMA1 Stream 5								*/
	void DMA1_Stream6_IRQHandler();				/* DMA1 Stream 6								*/
	void ADC_IRQHandler();						/* ADC1, ADC2 and ADC3s							*/
	void CAN1_TX_IRQHandler();					/* CAN1 TX										*/
	void CAN1_RX0_IRQHandler();					/* CAN1 RX0										*/
	void CAN1_RX1_IRQHandler();					/* CAN1 RX1										*/
	void CAN1_SCE_IRQHandler();					/* CAN1 SCE										*/
	void EXTI9_5_IRQHandler();					/* External Line[9:5]s							*/
	void TIM1_BRK_TIM9_IRQHandler();			/* TIM1 Break and TIM9							*/
	void TIM1_UP_TIM10_IRQHandler();			/* TIM1 Update and TIM10						*/
	void TIM1_TRG_COM_TIM11_IRQHandler();		/* TIM1 Trigger and Commutation and TIM11		*/
	void TIM1_CC_IRQHandler();					/* TIM1 Capture Compare							*/
	void TIM2_IRQHandler();						/* TIM2											*/
	void TIM3_IRQHandler();						/* TIM3											*/
	void TIM4_IRQHandler();						/* TIM4											*/
	void I2C1_EV_IRQHandler();					/* I2C1 Event									*/
	void I2C1_ER_IRQHandler();					/* I2C1 Error									*/
	void I2C2_EV_IRQHandler();					/* I2C2 Event									*/
	void I2C2_ER_IRQHandler();					/* I2C2 Error									*/
	void SPI1_IRQHandler();						/* SPI1											*/
	void SPI2_IRQHandler();						/* SPI2											*/
	void USART1_IRQHandler();					/* USART1										*/
	void USART2_IRQHandler();					/* USART2										*/
	void USART3_IRQHandler();					/* USART3										*/
	void EXTI15_10_IRQHandler();				/* External Line[15:10]s						*/
	void RTC_Alarm_IRQHandler();				/* RTC Alarm (A and B) through EXTI Line		*/
	void OTG_FS_WKUP_IRQHandler();				/* USB OTG FS Wakeup through EXTI line			*/
	void TIM8_BRK_TIM12_IRQHandler();			/* TIM8 Break and TIM12							*/
	void TIM8_UP_TIM13_IRQHandler();			/* TIM8 Update and TIM13						*/
	void TIM8_TRG_COM_TIM14_IRQHandler();		/* TIM8 Trigger and Commutation and TIM14		*/
	void TIM8_CC_IRQHandler();					/* TIM8 Capture Compare							*/
	void DMA1_Stream7_IRQHandler();				/* DMA1 Stream7									*/
	void FMC_IRQHandler();						/* FMC											*/
	void SDMMC1_IRQHandler();					/* SDMMC1										*/
	void TIM5_IRQHandler();						/* TIM5											*/
	void SPI3_IRQHandler();						/* SPI3											*/
	void UART4_IRQHandler();					/* UART4										*/
	void UART5_IRQHandler();					/* UART5										*/
	void TIM6_DAC_IRQHandler();					/* TIM6 and DAC1&2 underrun errors				*/
	void TIM7_IRQHandler();						/* TIM7											*/
	void DMA2_Stream0_IRQHandler();				/* DMA2 Stream 0								*/
	void DMA2_Stream1_IRQHandler();				/* DMA2 Stream 1								*/
	void DMA2_Stream2_IRQHandler();				/* DMA2 Stream 2								*/
	void DMA2_Stream3_IRQHandler();				/* DMA2 Stream 3								*/
	void DMA2_Stream4_IRQHandler();				/* DMA2 Stream 4								*/
	void ETH_IRQHandler();						/* Ethernet										*/
	void ETH_WKUP_IRQHandler();					/* Ethernet Wakeup through EXTI line			*/
	void CAN2_TX_IRQHandler();					/* CAN2 TX										*/
	void CAN2_RX0_IRQHandler();					/* CAN2 RX0										*/
	void CAN2_RX1_IRQHandler();					/* CAN2 RX1										*/
	void CAN2_SCE_IRQHandler();					/* CAN2 SCE										*/
	void OTG_FS_IRQHandler();					/* USB OTG FS									*/
	void DMA2_Stream5_IRQHandler();				/* DMA2 Stream 5								*/
	void DMA2_Stream6_IRQHandler();				/* DMA2 Stream 6								*/
	void DMA2_Stream7_IRQHandler();				/* DMA2 Stream 7								*/
	void USART6_IRQHandler();					/* USART6										*/
	void I2C3_EV_IRQHandler();					/* I2C3 event									*/
	void I2C3_ER_IRQHandler();					/* I2C3 error									*/
	void OTG_HS_EP1_OUT_IRQHandler();			/* USB OTG HS End Point 1 Out					*/
	void OTG_HS_EP1_IN_IRQHandler();			/* USB OTG HS End Point 1 In					*/
	void OTG_HS_WKUP_IRQHandler();				/* USB OTG HS Wakeup through EXTI				*/
	void OTG_HS_IRQHandler();					/* USB OTG HS									*/
	void DCMI_IRQHandler();						/* DCMI											*/
	void RNG_IRQHandler();						/* RNG											*/
	void FPU_IRQHandler();						/* FPU											*/
	void UART7_IRQHandler();					/* UART7										*/
	void UART8_IRQHandler();					/* UART8										*/
	void SPI4_IRQHandler();						/* SPI4											*/
	void SPI5_IRQHandler();						/* SPI5											*/
	void SPI6_IRQHandler();						/* SPI6											*/
	void SAI1_IRQHandler();						/* SAI1											*/
	void DMA2D_IRQHandler();					/* DMA2D										*/
	void SAI2_IRQHandler();						/* SAI2											*/
	void QUADSPI_IRQHandler();					/* QUADSPI										*/
	void LPTIM1_IRQHandler();					/* LPTIM1										*/
	void CEC_IRQHandler();						/* HDMI_CEC										*/
	void I2C4_EV_IRQHandler();					/* I2C4 Event									*/
	void I2C4_ER_IRQHandler();					/* I2C4 Error									*/
	void SPDIF_RX_IRQHandler();					/* SPDIF_RX										*/
	void DFSDM1_FLT0_IRQHandler();				/* DFSDM1 Filter 0 global Interrupt				*/
	void DFSDM1_FLT1_IRQHandler();				/* DFSDM1 Filter 1 global Interrupt				*/
	void DFSDM1_FLT2_IRQHandler();				/* DFSDM1 Filter 2 global Interrupt				*/
	void DFSDM1_FLT3_IRQHandler();				/* DFSDM1 Filter 3 global Interrupt				*/
	void SDMMC2_IRQHandler();					/* SDMMC2										*/
	void CAN3_TX_IRQHandler();					/* CAN3 TX										*/
	void CAN3_RX0_IRQHandler();					/* CAN3 RX0										*/
	void CAN3_RX1_IRQHandler();					/* CAN3 RX1										*/
	void CAN3_SCE_IRQHandler();					/* CAN3 SCE										*/
	void MDIOS_IRQHandler();					/* MDIOS										*/
}

using intfunc = void(*)();

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
	0,
	RNG_IRQHandler,
	FPU_IRQHandler,
	UART7_IRQHandler,
	UART8_IRQHandler,
	SPI4_IRQHandler,
	SPI5_IRQHandler,
	SPI6_IRQHandler,
	SAI1_IRQHandler,
	0,
	0,
	DMA2D_IRQHandler,
	SAI2_IRQHandler,
	QUADSPI_IRQHandler,
	LPTIM1_IRQHandler,
	CEC_IRQHandler,
	I2C4_EV_IRQHandler,
	I2C4_ER_IRQHandler,
	SPDIF_RX_IRQHandler,
	0,
	DFSDM1_FLT0_IRQHandler,
	DFSDM1_FLT1_IRQHandler,
	DFSDM1_FLT2_IRQHandler,
	DFSDM1_FLT3_IRQHandler,
	SDMMC2_IRQHandler,
	CAN3_TX_IRQHandler,
	CAN3_RX0_IRQHandler,
	CAN3_RX1_IRQHandler,
	CAN3_SCE_IRQHandler,
	0,
	MDIOS_IRQHandler
};

#include "default-hndl.hpp"

#pragma weak WWDG_IRQHandler				= Default_Handler
#pragma weak PVD_IRQHandler					= Default_Handler
#pragma weak TAMP_STAMP_IRQHandler			= Default_Handler
#pragma weak RTC_WKUP_IRQHandler			= Default_Handler
#pragma weak FLASH_IRQHandler				= Default_Handler
#pragma weak RCC_IRQHandler					= Default_Handler
#pragma weak EXTI0_IRQHandler				= Default_Handler
#pragma weak EXTI1_IRQHandler				= Default_Handler
#pragma weak EXTI2_IRQHandler				= Default_Handler
#pragma weak EXTI3_IRQHandler				= Default_Handler
#pragma weak EXTI4_IRQHandler				= Default_Handler
#pragma weak DMA1_Stream0_IRQHandler		= Default_Handler
#pragma weak DMA1_Stream1_IRQHandler		= Default_Handler
#pragma weak DMA1_Stream2_IRQHandler		= Default_Handler
#pragma weak DMA1_Stream3_IRQHandler		= Default_Handler
#pragma weak DMA1_Stream4_IRQHandler		= Default_Handler
#pragma weak DMA1_Stream5_IRQHandler		= Default_Handler
#pragma weak DMA1_Stream6_IRQHandler		= Default_Handler
#pragma weak ADC_IRQHandler					= Default_Handler
#pragma weak CAN1_TX_IRQHandler				= Default_Handler
#pragma weak CAN1_RX0_IRQHandler			= Default_Handler
#pragma weak CAN1_RX1_IRQHandler			= Default_Handler
#pragma weak CAN1_SCE_IRQHandler			= Default_Handler
#pragma weak EXTI9_5_IRQHandler				= Default_Handler
#pragma weak TIM1_BRK_TIM9_IRQHandler		= Default_Handler
#pragma weak TIM1_UP_TIM10_IRQHandler		= Default_Handler
#pragma weak TIM1_TRG_COM_TIM11_IRQHandler	= Default_Handler
#pragma weak TIM1_CC_IRQHandler				= Default_Handler
#pragma weak TIM2_IRQHandler				= Default_Handler
#pragma weak TIM3_IRQHandler				= Default_Handler
#pragma weak TIM4_IRQHandler				= Default_Handler
#pragma weak I2C1_EV_IRQHandler				= Default_Handler
#pragma weak I2C1_ER_IRQHandler				= Default_Handler
#pragma weak I2C2_EV_IRQHandler				= Default_Handler
#pragma weak I2C2_ER_IRQHandler				= Default_Handler
#pragma weak SPI1_IRQHandler				= Default_Handler
#pragma weak SPI2_IRQHandler				= Default_Handler
#pragma weak USART1_IRQHandler				= Default_Handler
#pragma weak USART2_IRQHandler				= Default_Handler
#pragma weak USART3_IRQHandler				= Default_Handler
#pragma weak EXTI15_10_IRQHandler			= Default_Handler
#pragma weak RTC_Alarm_IRQHandler			= Default_Handler
#pragma weak OTG_FS_WKUP_IRQHandler			= Default_Handler
#pragma weak TIM8_BRK_TIM12_IRQHandler		= Default_Handler
#pragma weak TIM8_UP_TIM13_IRQHandler		= Default_Handler
#pragma weak TIM8_TRG_COM_TIM14_IRQHandler	= Default_Handler
#pragma weak TIM8_CC_IRQHandler				= Default_Handler
#pragma weak DMA1_Stream7_IRQHandler		= Default_Handler
#pragma weak FMC_IRQHandler					= Default_Handler
#pragma weak SDMMC1_IRQHandler				= Default_Handler
#pragma weak TIM5_IRQHandler				= Default_Handler
#pragma weak SPI3_IRQHandler				= Default_Handler
#pragma weak UART4_IRQHandler				= Default_Handler
#pragma weak UART5_IRQHandler				= Default_Handler
#pragma weak TIM6_DAC_IRQHandler			= Default_Handler
#pragma weak TIM7_IRQHandler				= Default_Handler
#pragma weak DMA2_Stream0_IRQHandler		= Default_Handler
#pragma weak DMA2_Stream1_IRQHandler		= Default_Handler
#pragma weak DMA2_Stream2_IRQHandler		= Default_Handler
#pragma weak DMA2_Stream3_IRQHandler		= Default_Handler
#pragma weak DMA2_Stream4_IRQHandler		= Default_Handler
#pragma weak ETH_IRQHandler					= Default_Handler
#pragma weak ETH_WKUP_IRQHandler			= Default_Handler
#pragma weak CAN2_TX_IRQHandler				= Default_Handler
#pragma weak CAN2_RX0_IRQHandler			= Default_Handler
#pragma weak CAN2_RX1_IRQHandler			= Default_Handler
#pragma weak CAN2_SCE_IRQHandler			= Default_Handler
#pragma weak OTG_FS_IRQHandler				= Default_Handler
#pragma weak DMA2_Stream5_IRQHandler		= Default_Handler
#pragma weak DMA2_Stream6_IRQHandler		= Default_Handler
#pragma weak DMA2_Stream7_IRQHandler		= Default_Handler
#pragma weak USART6_IRQHandler				= Default_Handler
#pragma weak I2C3_EV_IRQHandler				= Default_Handler
#pragma weak I2C3_ER_IRQHandler				= Default_Handler
#pragma weak OTG_HS_EP1_OUT_IRQHandler		= Default_Handler
#pragma weak OTG_HS_EP1_IN_IRQHandler		= Default_Handler
#pragma weak OTG_HS_WKUP_IRQHandler			= Default_Handler
#pragma weak OTG_HS_IRQHandler				= Default_Handler
#pragma weak DCMI_IRQHandler				= Default_Handler
#pragma weak RNG_IRQHandler					= Default_Handler
#pragma weak FPU_IRQHandler					= Default_Handler
#pragma weak UART7_IRQHandler				= Default_Handler
#pragma weak UART8_IRQHandler				= Default_Handler
#pragma weak SPI4_IRQHandler				= Default_Handler
#pragma weak SPI5_IRQHandler				= Default_Handler
#pragma weak SPI6_IRQHandler				= Default_Handler
#pragma weak SAI1_IRQHandler				= Default_Handler
#pragma weak DMA2D_IRQHandler				= Default_Handler
#pragma weak SAI2_IRQHandler				= Default_Handler
#pragma weak QUADSPI_IRQHandler				= Default_Handler
#pragma weak LPTIM1_IRQHandler				= Default_Handler
#pragma weak CEC_IRQHandler					= Default_Handler
#pragma weak I2C4_EV_IRQHandler				= Default_Handler
#pragma weak I2C4_ER_IRQHandler				= Default_Handler
#pragma weak SPDIF_RX_IRQHandler			= Default_Handler
#pragma weak DFSDM1_FLT0_IRQHandler			= Default_Handler
#pragma weak DFSDM1_FLT1_IRQHandler			= Default_Handler
#pragma weak DFSDM1_FLT2_IRQHandler			= Default_Handler
#pragma weak DFSDM1_FLT3_IRQHandler			= Default_Handler
#pragma weak SDMMC2_IRQHandler				= Default_Handler
#pragma weak CAN3_TX_IRQHandler				= Default_Handler
#pragma weak CAN3_RX0_IRQHandler			= Default_Handler
#pragma weak CAN3_RX1_IRQHandler			= Default_Handler
#pragma weak CAN3_SCE_IRQHandler			= Default_Handler
#pragma weak MDIOS_IRQHandler				= Default_Handler
