#include <stm32f7xx.h>
#include <stm32f7xx_hal.h>
#include <commdef.hpp>

namespace core {
	void InitCPU();
}

uint32_t SystemCoreClock = 216'000'000;
const uint8_t AHBPrescTable[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9 };
const uint8_t APBPrescTable[8] = { 0, 0, 0, 0, 1, 2, 3, 4 };

/*
==================
core::InitCPU

	Configure clocks, CPU cache and flash interface.
==================
*/
void core::InitCPU() {
	//Enable Bus Fault, Memory Fault, Usage Fault exceptions
	SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk);

	// Enable FPU
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // set CP10 and CP11 Full Access
#endif

	// Reset the RCC clock configuration to the default reset state
	RCC->CR |= uint32_t(0x00'00'00'01);		// Set HSION bit

	// Reset CFGR register
	RCC->CFGR = 0x00'00'00'00;

	// Reset HSEON, CSSON and PLLON bits
	RCC->CR &= uint32_t(0xFE'F6'FF'FF);

	// Reset PLLCFGR register
	RCC->PLLCFGR = 0x24'00'30'10;

	// Reset HSEBYP bit
	RCC->CR &= uint32_t(0xFF'FB'FF'FF);

	// Disable all interrupts
	RCC->CIR = 0x00'00'00'00;

	// Configure the Vector Table location add offset address
	SCB->VTOR = FLASH_BASE;					// Vector Table Relocation in Internal FLASH

	// Enable branch prediction
	SCB->CCR |= (1 << 18);
	__DSB();

	// Enable instruction and data cache
	SCB_EnableICache();
	SCB_EnableDCache();

	RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_ClkInitTypeDef RCC_ClkInitStruct;

	// Configure the main internal regulator output voltage
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	// Configure PLL to provide 216 MHz on PLLP
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 12;		//For HSE crystal 12MHz
	RCC_OscInitStruct.PLL.PLLN = 432;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	// Activate the Over-Drive mode for 3.3v and 216 MHz
	HAL_PWREx_EnableOverDrive();

	// Switch CPU, AHB and APB busses to 216 MHz SYSCLK
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK	| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7);	// 7 WS for 3.3v 216MHz
}
