#include <stm32f7xx.h>
#include "procsdef.hpp"
#include "commdef.hpp"


namespace core {
	[[noreturn]] void Main();
}

uint32_t SystemCoreClock = 16'000'000;
const uint8_t AHBPrescTable[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9 };
const uint8_t APBPrescTable[8] = { 0, 0, 0, 0, 1, 2, 3, 4 };

/*
==================
core::Main

	Init all devices and start RTOS.
==================
*/
void core::Main() {


	OS::run();
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


static BackLightCtrl g_backLightCtrl;

/*
==================
BackLightCtrl::exec
==================
*/
template<> void BackLightCtrl::exec() {
	OS::sleep();
	__builtin_unreachable();
}

/*
==================
abort
==================
*/
MANGOFF void abort() {
	if constexpr (g_debug) {
		__asm("bkpt #0");
	} else {
		while (true);
	}
	__builtin_unreachable();
}
