#ifndef SRC_SYSTIME_H_
#define SRC_SYSTIME_H_

volatile uint32_t msTicks; /* counts 1ms timeTicks */

void Delay(uint32_t milliseconds);

void SysTick_Handler(void) {
	msTicks++; // increment counter necessary in Delay()
}

void Delay(uint32_t milliseconds) {
	uint32_t curTicks;

	curTicks = msTicks;
	while ((msTicks - curTicks) < milliseconds)
		;
}

#endif /* SRC_SYSTIME_H_ */
