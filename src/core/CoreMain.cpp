#include <stm32f7xx.h>
#include "procsdef.hpp"
#include "commdef.hpp"
#include "drv/lcd.hpp"
#include "../3rdParty/FatFs/ff.h"


namespace core {
	[[noreturn]] void Main();
}

int main(int argc, char **argv);


static core::UserApp g_userApp;
static FATFS g_fs;


/*
==================
core::Main

	Init all devices and start RTOS.
==================
*/
void core::Main() {
	core::InitMPU();
	core::InitLcdInterface();
	core::InitLcdBacklight();
	core::InitLcd();

	OS::run();
}

/*
==================
core::UserApp::exec

	call app ctors, change CWD and call main
==================
*/
template<> void core::UserApp::exec() {
	core::LcdRunTimer();
	uint8_t r = 0, g = 40, b = 200;

	while (1) {
		//core::FillLcdRandom();
		GPIOA->BSRR = GPIO_BSRR_BS_15;
		r++;
		g -= 35;
		b += 10;
		core::LcdFillColor(((b & 0b0001'1111) << 11) | ((g & 0b0011'1111) << 5) | (r & 0b0001'1111));
		GPIOA->BSRR = GPIO_BSRR_BR_15;
		core::LcdSync();
	}

	if (f_mount(&g_fs, "BSD:/", 1) != FR_OK) {
		if (f_mount(&g_fs, "TSD:/", 1) != FR_OK) {
			EXCEPT_HNDL()
		} else {
			f_chdir("TSD:/");
		}
	} else {
		f_chdir("BSD:/");
	}

	main(0, nullptr);

	OS::sleep();
	__builtin_unreachable();
}

/*
==================
DelayMs

	Delay function for pre-rtos code
==================
*/
void DelayMs(const unsigned int ms) {
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;

	constexpr unsigned int psc = (SYSTICKFREQ / 2) / 10'000 - 1;		//tim5 on APB1 bus

	TIM5->DIER &= ~(TIM_DIER_UIE);
	TIM5->PSC = psc;	//timer clock 1Khz
	TIM5->ARR = ms * 10 - 1;
	TIM5->CNT = 0;
	TIM5->CR1 = TIM_CR1_OPM | TIM_CR1_CEN;

	while (!(TIM5->SR & TIM_SR_UIF));	//wait
	TIM5->SR &= ~(TIM_SR_UIF);

	RCC->APB1ENR &= ~(RCC_APB1ENR_TIM5EN);
}

/*
==================
OS::idle_process_user_hook
==================
*/
#if scmRTOS_IDLE_HOOK_ENABLE
void OS::idle_process_user_hook() {
	__WFI();
}
#endif

/*
==================
_sbrk
==================
*/
extern "C" caddr_t _sbrk(int incr) {
	extern char sdramBegin, sdramEnd;		// Defined by the linker
	static char *heapEnd = &sdramBegin;
	char *prevHeapEnd = heapEnd;

	if (unlikely(heapEnd + incr > &sdramEnd)) {
		EXCEPT_HNDL()
	}

	heapEnd += incr;
	return caddr_t(prevHeapEnd);
}

/*
==================
__cxa_pure_virtual
==================
*/
extern "C" void __cxa_pure_virtual() {
	EXCEPT_HNDL()
}

/*
==================
abort
==================
*/
extern "C" void abort() {
	EXCEPT_HNDL()
	__builtin_unreachable();
}

#pragma region Hardware exceptions
extern "C" {
	/*
	==================
	HardFault_Handler
	==================
	*/
	void HardFault_Handler() {
		EXCEPT_HNDL()
	}

	/*
	==================
	MemManage_Handler
	==================
	*/
	void MemManage_Handler() {
		EXCEPT_HNDL()
	}

	/*
	==================
	BusFault_Handler
	==================
	*/
	void BusFault_Handler() {
		EXCEPT_HNDL()
	}

	/*
	==================
	UsageFault_Handler
	==================
	*/
	void UsageFault_Handler() {
		EXCEPT_HNDL()
	}
}
#pragma endregion