#include "main.h"
#include "systime.h"
#include "effects.h"

void GPIO_Init() {
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
}

int main(void) {
	/* Chip errata */
	CHIP_Init();

	GPIO_Init();

	/* Initialize HFXO with specific parameters */
	CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
	CMU_HFXOInit(&hfxoInit);

	/* Enable and set HFXO for HFCLK */
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

	DISPLAY_Init();

	/*
	 * Version 1
	 * Retargeting stdio to LCD
	 *
	 * */

	/* Retarget stdio to a text display. */
	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
		while (1)
			;
	}
	printf("\n");
	printf("Hello World!");
	/*
	 * Version 2
	 * Text display driver's native text output function
	 *
	 * */

	// Clear LED to see that program did not fail
	GPIO_PinOutClear(gpioPortC, 5);
	setup_effects();

	while (1) {
		// TEXTDISPLAY_WriteString(textHandle, "Hello world!");
		printf("Hello world!");
	}
}
