#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"

#include <stdio.h>

#include "hal-config.h"

#include "textdisplay.h"
#include "retargettextdisplay.h"
#include "display.h"

//DISPLAY_Device_t displayDevice;
//TEXTDISPLAY_Config_t displayConf;
//TEXTDISPLAY_Handle_t textHandle;

int main(void) {
	/* Chip errata */
	CHIP_Init();

	/* Infinite loop */
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Setup LEDs */
	GPIO_PinModeSet(gpioPortC, 4, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortC, 5, gpioModePushPull, 0);

	/* Turn on LEDs */
	GPIO_PinOutSet(gpioPortC, 4);
	GPIO_PinOutSet(gpioPortC, 5);

	/* Setup buttons */
	// Top button
	GPIO_PinModeSet(gpioPortA, 0, gpioModeInput, 0);

	DISPLAY_Init();

	/*
	 * Version 1
	 * Retargeting stdio to LCD
	 *
	 * */

	/* Retarget stdio to a text display. */
//	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
//	  while (1);
//	}
//
//	printf("\n");
//	printf("Hello World!");

	/*
	 * Version 2
	 * Text display driver's native text output function
	 *
	 * */

	DISPLAY_Device_t displayDevice;
	TEXTDISPLAY_Config_t displayConf;
	TEXTDISPLAY_Handle_t textHandle;

	/* Retrieve the properties of the display. */
	if (DISPLAY_DeviceGet ( 0, &displayDevice ) != DISPLAY_EMSTATUS_OK)
	{
	    /* Unable to get display handle. */
	    while (1);
	}

	/* Setup config struct for TEXTDISPLAY */
	displayConf.displayDeviceNo = 0;
	displayConf.lfToCrLf = true;
	displayConf.scrollEnable = false;

	/* Create a TEXTDISPLAY device */
	TEXTDISPLAY_New (&displayConf, &textHandle);
	// Write to LCD screen
	TEXTDISPLAY_WriteString (textHandle, "Hello world!");

	// Clear LED to see that program did not fail
	GPIO_PinOutClear(gpioPortC, 5);

	while (1) {
		// Listen for button click
		if (!GPIO_PinInGet(gpioPortA, 0)) {
			while (!GPIO_PinInGet(gpioPortA, 0))
				;
			// Toggle LED
			GPIO_PinOutToggle(gpioPortC, 4);
			printf("hey");
		}
	}
}
