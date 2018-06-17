#include "../procsdef.hpp"
#include "lcd.hpp"
#include <stm32f7xx.h>


static core::BackLightCtrl g_backLightCtrl;


/*
==================
core::InitLcdBacklight
==================
*/
void core::InitLcdBacklight() {

}

/*
==================
core::BackLightCtrl::exec
==================
*/
template<> void core::BackLightCtrl::exec() {
	OS::sleep();
	while (1) {

	}
}
