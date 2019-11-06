#include "main.h"
#include "effects.h"
#include "systime.h"
#include "lcd.h"

void GPIO_Init() {
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Setup LEDs */
	GPIO_PinModeSet(gpioPortC, 4, gpioModePushPull, 0);
	GPIO_PinModeSet(gpioPortC, 5, gpioModePushPull, 0);

	/* Turn on LEDs */
	GPIO_PinOutSet(gpioPortC, 4);
	GPIO_PinOutSet(gpioPortC, 5);

	/* Setup buttons */
	for (int i = 0; i < 6; i++) {
		GPIO_PinModeSet(gpioPortA, i, gpioModeInput, 0);
	}
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

	bool initialRender = true;

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

	// Clear LED to see that program did not fail
	GPIO_PinOutClear(gpioPortC, 5);
	setup_effects();

	while (1) {

		// Print list of effects with their respective initial values
		if (initialRender) {
			LCD_InitialRender();
			initialRender = false;
		}

		// Top navigation button
		if (!GPIO_PinInGet(gpioPortA, 0)) {
			while (!GPIO_PinInGet(gpioPortA, 0))
				;
			LCD_NavigateUp();
		}

		// Buttom navigation button
		if (!GPIO_PinInGet(gpioPortA, 2)) {
			while (!GPIO_PinInGet(gpioPortA, 2))
				;
			LCD_NavigateDown();
		}

		// Left decrement button
		if (!GPIO_PinInGet(gpioPortA, 5)) {
			while (!GPIO_PinInGet(gpioPortA, 5))
				;
			LCD_DecrementValue();
		}

		// Right increment button
		if (!GPIO_PinInGet(gpioPortA, 4)) {
			while (!GPIO_PinInGet(gpioPortA, 4))
				;
			LCD_IncrementValue();
		}

	}
}
