#include <stm32f7xx.h>


namespace core {
	void InitSDRAM();
}

/*
==================
core::InitSDRAM

	WARNING! This code runs before any of .bss and .data sections initialized!
==================
*/
void core::InitSDRAM() {
	__IO uint32_t tmp = 0;
	uint32_t tmpreg = 0, timeout = 0xFFFF, index;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN | RCC_AHB1ENR_GPIOEEN | RCC_AHB1ENR_GPIOFEN | RCC_AHB1ENR_GPIOGEN;
	
	/* Delay after an RCC peripheral clock enabling */
	tmp = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN);

	//PA7
	/* Connect PAx pins to FMC Alternate function */
	GPIOA->AFR[0] = 0xC0000000;
	GPIOA->AFR[1] = 0x00000000;
	/* Configure PAx pins in Alternate function mode */
	GPIOA->MODER = 0x00008000;
	/* Configure PAx pins speed to 100 MHz */
	GPIOA->OSPEEDR = 0x0000C000;
	/* Configure PAx pins Output type to push-pull */
	GPIOA->OTYPER = 0x00000000;
	/* No pull-up and no pull-down for PAx pins */
	GPIOA->PUPDR = 0x00000000;

	//PC4, PC5
	/* Connect PCx pins to FMC Alternate function */
	GPIOC->AFR[0] = 0x00CC0000;
	GPIOC->AFR[1] = 0x00000000;
	/* Configure PCx pins in Alternate function mode */
	GPIOC->MODER = 0x00000A00;
	/* Configure PCx pins speed to 100 MHz */
	GPIOC->OSPEEDR = 0x00000F00;
	/* Configure PCx pins Output type to push-pull */
	GPIOC->OTYPER = 0x00000000;
	/* No pull-up and no pull-down for PCx pins */
	GPIOC->PUPDR = 0x00000000;

	//PD0, PD1, PD8-10, PD14, PD15
	/* Connect PDx pins to FMC Alternate function */
	GPIOD->AFR[0] = 0x000000CC;
	GPIOD->AFR[1] = 0xCC000CCC;
	/* Configure PDx pins in Alternate function mode */
	GPIOD->MODER = 0xA02A000A;
	/* Configure PDx pins speed to 100 MHz */
	GPIOD->OSPEEDR = 0xF03F000F;
	/* Configure PDx pins Output type to push-pull */
	GPIOD->OTYPER = 0x00000000;
	/* No pull-up and no pull-down for PDx pins */
	GPIOD->PUPDR = 0x00000000;

	//PE0, PE1, PE7-PE15
	/* Connect PEx pins to FMC Alternate function */
	GPIOE->AFR[0] = 0xC00000CC;
	GPIOE->AFR[1] = 0xCCCCCCCC;
	/* Configure PEx pins in Alternate function mode */
	GPIOE->MODER = 0xAAAA800A;
	/* Configure PEx pins speed to 100 MHz */
	GPIOE->OSPEEDR = 0xFFFFC00F;
	/* Configure PEx pins Output type to push-pull */
	GPIOE->OTYPER = 0x00000000;
	/* No pull-up and no pull-down for PEx pins */
	GPIOE->PUPDR = 0x00000000;

	//PF0-PF5, PF11-PF15
	/* Connect PFx pins to FMC Alternate function */
	GPIOF->AFR[0] = 0x00CCCCCC;
	GPIOF->AFR[1] = 0xCCCCC000;
	/* Configure PFx pins in Alternate function mode */
	GPIOF->MODER = 0xAA800AAA;
	/* Configure PFx pins speed to 100 MHz */
	GPIOF->OSPEEDR = 0xFFC00FFF;
	/* Configure PFx pins Output type to push-pull */
	GPIOF->OTYPER = 0x00000000;
	/* No pull-up and no pull-down for PFx pins */
	GPIOF->PUPDR = 0x00000000;

	//PG0-PG2, PG4, PG5, PG8, PG15
	/* Connect PGx pins to FMC Alternate function */
	GPIOG->AFR[0] = 0x00CC0CCC;
	GPIOG->AFR[1] = 0xC000000C;
	/* Configure PGx pins in Alternate function mode */
	GPIOG->MODER = 0x80020A2A;
	/* Configure PGx pins speed to 100 MHz */
	GPIOG->OSPEEDR = 0xC0030F3F;
	/* Configure PGx pins Output type to push-pull */
	GPIOG->OTYPER = 0x00000000;
	/* No pull-up and no pull-down for PGx pins */
	GPIOG->PUPDR = 0x00000000;

	/*-- FMC Configuration ------------------------------------------------------*/
	/* Enable the FMC interface clock */
	RCC->AHB3ENR |= RCC_AHB3ENR_FMCEN;

	/* Delay after an RCC peripheral clock enabling */
	tmp = READ_BIT(RCC->AHB3ENR, RCC_AHB3ENR_FMCEN);

	/* Configure and enable SDRAM bank1 */
	FMC_Bank5_6->SDCR[0] = 0x000019E5;
	FMC_Bank5_6->SDTR[0] = 0x01116361;

	/* SDRAM initialization sequence ---------------------------------------------*/
	/* Clock enable command */
	FMC_Bank5_6->SDCMR = 0x00000011;
	tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	while ((tmpreg != 0) && (timeout-- > 0)) {
		tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	}

	/* Delay */
	for (index = 0; index<1000; index++);

	/* PALL command */
	FMC_Bank5_6->SDCMR = 0x00000012;
	timeout = 0xFFFF;
	while ((tmpreg != 0) && (timeout-- > 0)) {
		tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	}

	/* Auto refresh command */
	FMC_Bank5_6->SDCMR = 0x000000F3;
	timeout = 0xFFFF;
	while ((tmpreg != 0) && (timeout-- > 0)) {
		tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	}

	/* MRD register program */
	FMC_Bank5_6->SDCMR = 0x00046014;
	timeout = 0xFFFF;
	while ((tmpreg != 0) && (timeout-- > 0)) {
		tmpreg = FMC_Bank5_6->SDSR & 0x00000020;
	}

	/* Set refresh count */
	tmpreg = FMC_Bank5_6->SDRTR;
	FMC_Bank5_6->SDRTR = (tmpreg | (0x00000603 << 1));

	/* Disable write protection */
	tmpreg = FMC_Bank5_6->SDCR[0];
	FMC_Bank5_6->SDCR[0] = (tmpreg & 0xFFFFFDFF);
}
