#include <stm32f7xx.h>

namespace core {
	void InitCPU();
	void InitClock();
}

/*
==================
core::InitCPU

	Init Cortex-M7 CPU
==================
*/
void core::InitCPU() {
	//Enable Bus Fault, Memory Fault, Usage Fault exceptions
	SCB->SHCSR |= (SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk | SCB_SHCSR_USGFAULTENA_Msk);

	// Enable FPU
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));  // set CP10 and CP11 Full Access
#endif

	// Configure the Vector Table location add offset address
	SCB->VTOR = FLASH_BASE;					// Vector Table Relocation in Internal FLASH

	// Enable instruction and data cache
	SCB_EnableICache();
	SCB_EnableDCache();
}

/*
==================
core::InitClock

	Init clock generator and bus dividers
==================
*/
void core::InitClock() {
#pragma region RCC reset
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
#pragma endregion

	//Power config
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);				//enable power controller
	volatile uint32_t tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);

	MODIFY_REG(PWR->CR1, PWR_CR1_VOS, (PWR_CR1_VOS));		//SCALE1 mode
	tmpreg = READ_BIT(PWR->CR1, PWR_CR1_VOS);

	//HSE config
	SET_BIT(RCC->CR, RCC_CR_HSEON);				//enable HSE
	while (!READ_BIT(RCC->CR, RCC_CR_HSERDY));	//wait till HSE ready

	//PLL config
	RCC->PLLCFGR = (9 << RCC_PLLCFGR_PLLQ_Pos) | (0 << RCC_PLLCFGR_PLLP_Pos) | (432 << RCC_PLLCFGR_PLLN_Pos) | (8 << RCC_PLLCFGR_PLLM_Pos) | RCC_PLLCFGR_PLLSRC;
	
	SET_BIT(RCC->CR, RCC_CR_PLLON);		//Enable main PLL

	//Over-drive mode enable
	SET_BIT(PWR->CR1, PWR_CR1_ODEN);
	while (!READ_BIT(PWR->CSR1, PWR_CSR1_ODRDY));

	SET_BIT(PWR->CR1, PWR_CR1_ODSWEN);
	while (!READ_BIT(PWR->CSR1, PWR_CSR1_ODSWRDY));

	//Set flash latency
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_7WS);
	tmpreg = FLASH->ACR;

	//Config buses clocks
	constexpr uint32_t cfgrClearMask = RCC_CFGR_PPRE2 | RCC_CFGR_PPRE1 | RCC_CFGR_HPRE;
	constexpr uint32_t cfgrValue =	(0b100 << RCC_CFGR_PPRE2_Pos) |			//APB2 prescaller 2
									(0b101 << RCC_CFGR_PPRE1_Pos) |			//APB1 prescaller 4
									(0 << RCC_CFGR_HPRE_Pos);				//AHB prescaller 1
	MODIFY_REG(RCC->CFGR, cfgrClearMask, cfgrValue);

	//wait for PLL lock
	while (!READ_BIT(RCC->CR, RCC_CR_PLLRDY));

	//select PLL as clock source
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, (0b10 << RCC_CFGR_SW_Pos));
	while ((RCC->CFGR & RCC_CFGR_SWS) != (0b10 << RCC_CFGR_SWS_Pos));

	//Select PLLQ as CLK48 source, CLK48 as SDMMC1 and SDMMC2 clock
	MODIFY_REG(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL | RCC_DCKCFGR2_SDMMC1SEL | RCC_DCKCFGR2_SDMMC2SEL, 0);
}
