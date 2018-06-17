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
	core::InitLcdBacklight();

	OS::run();
}

/*
==================
core::UserApp::exec

	call app ctors, change CWD and call main
==================
*/
template<> void core::UserApp::exec() {
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

	if (heapEnd + incr > &sdramEnd) {
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