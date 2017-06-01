#pragma once

#include <commdef.hpp>

/*
==================
Default_Handler
==================
*/
MANGOFF void Default_Handler() {
	if constexpr (g_debug) {
		__asm("bkpt #0");
	} else {
		while (true);
	}
}
