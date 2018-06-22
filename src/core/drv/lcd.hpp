#pragma once

namespace core {
	//caled before RTOS started
	void InitLcdBacklight();
	void InitLcdInterface();
	void InitMPU();
	void InitLcd();
	

	//called from rtos processes
	void LcdRunTimer();
	void LcdSync();
	void FillLcdRandom();
	void LcdFillColor(const uint16_t color);
}
