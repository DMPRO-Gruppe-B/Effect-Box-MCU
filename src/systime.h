#ifndef SRC_SYSTIME_H_
#define SRC_SYSTIME_H_

#include <sys/_stdint.h>

volatile uint32_t msTicks; /* counts 1ms timeTicks */

void Delay(uint32_t milliseconds);

void SysTick_Handler(void);

void Delay(uint32_t milliseconds);

#endif /* SRC_SYSTIME_H_ */
