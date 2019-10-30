#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"

#include <stdio.h>

#include "hal-config.h"

#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"

int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* Infinite loop */
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_PinModeSet(gpioPortC, 4, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortC, 5, gpioModePushPull, 0);

	GPIO_PinOutSet(gpioPortC, 4);
	GPIO_PinOutSet(gpioPortC, 5);

	DISPLAY_Init();

	/* Retarget stdio to a text display. */
	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
	  while (1);
	}

	printf("Hello World!");

	while (1) {
	}
}
