#include <commdef.hpp>

using intfunc = void(*)();

MANGOFF void __libc_init_array();

/* CPU interrupt vectors */
extern "C" {
	void NMI_Handler();
	void HardFault_Handler();
	void MemManage_Handler();
	void BusFault_Handler();
	void UsageFault_Handler();
	void SVC_Handler();
	void DebugMon_Handler();
	void PendSV_Handler();
	void SysTick_Handler();
	[[noreturn]] void Reset_Handler();
}

namespace core {
	void InitClocks();
	[[noreturn]] void Main();
}

extern unsigned int _estack;

[[gnu::used, gnu::section(".isr_vector_cpu")]] static const intfunc g_cpuVectors[] = {
	reinterpret_cast<intfunc>(&_estack),
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler
};

/*
==================
Reset_Handler
==================
*/
MANGOFF void Reset_Handler() {
	extern unsigned int _sidata, _sdata, _edata, _sbss, _ebss;
	unsigned int *pulSrc = &_sidata;

	/* Copy .data section */
	for (unsigned int *pulDest = &_sdata; pulDest < &_edata; ) {
		*(pulDest++) = *(pulSrc++);
	}

	/* Clear .bss section */
	for (unsigned int *pulDest = &_sbss; pulDest < &_ebss; ) {
		*(pulDest++) = 0;
	}

	core::InitClocks();

	/* Call global constructors from .init_array and .preinit_array sections */
	__libc_init_array();

	core::Main();
}

#include "default-hndl.hpp"

#pragma weak NMI_Handler		= Default_Handler
#pragma weak HardFault_Handler	= Default_Handler
#pragma weak MemManage_Handler	= Default_Handler
#pragma weak BusFault_Handler	= Default_Handler
#pragma weak UsageFault_Handler = Default_Handler
#pragma weak SVC_Handler		= Default_Handler
#pragma weak DebugMon_Handler	= Default_Handler
#pragma weak PendSV_Handler		= Default_Handler
#pragma weak SysTick_Handler	= Default_Handler
