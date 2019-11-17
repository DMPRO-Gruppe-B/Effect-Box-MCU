#include "systime.h"

void SysTick_Handler(void) {
	msTicks++; // increment counter necessary in Delay()
}

void Delay(uint32_t milliseconds) {
	uint32_t curTicks;

	curTicks = msTicks;
	while ((msTicks - curTicks) < milliseconds)
		;
}
