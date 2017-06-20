#include <stm32f7xx.h>
#include "procsdef.hpp"
#include "commdef.hpp"


namespace core {
	[[noreturn]] void Main();
}


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
__cxa_pure_virtual
==================
*/
MANGOFF void __cxa_pure_virtual() {
	EXCEPT_HNDL()
}

/*
==================
abort
==================
*/
MANGOFF void abort() {
	EXCEPT_HNDL()
	__builtin_unreachable();
}

/*
==================
HardFault_Handler
==================
*/
MANGOFF void HardFault_Handler() {
	EXCEPT_HNDL()
}

/*
==================
MemManage_Handler
==================
*/
MANGOFF void MemManage_Handler() {
	EXCEPT_HNDL()
}

/*
==================
BusFault_Handler
==================
*/
MANGOFF void BusFault_Handler() {
	EXCEPT_HNDL()
}

/*
==================
UsageFault_Handler
==================
*/
MANGOFF void UsageFault_Handler() {
	EXCEPT_HNDL()
}
