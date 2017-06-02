#include <scmRTOS.h>
#include <stm32f7xx.h>

namespace core {
	void Main();
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
