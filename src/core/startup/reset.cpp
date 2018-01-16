#include <commdef.hpp>

#define WEAK_ISR	[[gnu::weak, gnu::alias("Default_Handler")]]

MANGOFF void Default_Handler();
MANGOFF [[noreturn]] void Reset_Handler();
MANGOFF void __libc_init_array();

namespace core {
	void InitCPU();
	[[noreturn]] void Main();
}

/* CPU interrupt vectors */
MANGOFF {
	WEAK_ISR void NMI_Handler();
	WEAK_ISR void HardFault_Handler();
	WEAK_ISR void MemManage_Handler();
	WEAK_ISR void BusFault_Handler();
	WEAK_ISR void UsageFault_Handler();
	WEAK_ISR void SVC_Handler();
	WEAK_ISR void DebugMon_Handler();
	WEAK_ISR void PendSV_Handler();
	WEAK_ISR void SysTick_Handler();
}

extern unsigned int _estack;		//defined in linker script

using intfunc = void(*)();

[[gnu::used, gnu::section(".isr_vector_cpu")]] static const intfunc g_cpuVectors[] = {
	reinterpret_cast<intfunc>(&_estack),
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	nullptr,
	nullptr,
	nullptr,
	nullptr,
	SVC_Handler,
	DebugMon_Handler,
	nullptr,
	PendSV_Handler,
	SysTick_Handler
};

static_assert(sizeof g_cpuVectors / sizeof(*g_cpuVectors) == 16, "Invalid size of g_cpuVectors[]!");

/*
==================
Reset_Handler
==================
*/
void Reset_Handler() {
	extern unsigned int _sidata, _sdata, _edata, _sbss, _ebss;		//defined in linker script
	auto pulSrc = &_sidata;

	/* Copy .data section */
	for (auto pulDest = &_sdata; pulDest < &_edata; ) {
		*(pulDest++) = *(pulSrc++);
	}

	/* Clear .bss section */
	for (auto pulDest = &_sbss; pulDest < &_ebss; ) {
		*(pulDest++) = 0;
	}

	/* Call global constructors from .init_array and .preinit_array sections */
	__libc_init_array();

	core::Main();
}

/*
==================
_init

	__libc_init_array callback
==================
*/
MANGOFF void _init() {
	core::InitCPU();
}

/*
==================
Default_Handler
==================
*/
void Default_Handler() {
	EXCEPT_HNDL()
}
