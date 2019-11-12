#include "main.h"

#include <core_cm3.h>
#include <em_gpio.h>
#include <stdbool.h>
#include <sys/_stdint.h>

#include "effects.h"
#include "lcd.h"
#include "systime.h"

void GPIO_Init() {
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Setup LEDs */
	GPIO_PinModeSet(LED_PORT, LED_LEFT, gpioModePushPull, 0);
	GPIO_PinModeSet(LED_PORT, LED_RIGHT, gpioModePushPull, 0);

	/* Turn on LEDs */
	GPIO_PinOutSet(LED_PORT, LED_LEFT);
	GPIO_PinOutSet(LED_PORT, LED_RIGHT);

	/* Setup buttons */

	GPIO_PinModeSet(BUTTON_PORT, BUTTON_NAVIGATION_UP, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_NAVIGATION_DOWN, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_NAVIGATION_LEFT, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_NAVIGATION_RIGHT, gpioModeInput, 0);

	GPIO_PinModeSet(BUTTON_PORT, BUTTON_INCREMENT, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_DECREMENT, gpioModeInput, 0);

}

/* Counts 1ms timeTicks */
volatile uint32_t lastButton = 0;
volatile uint32_t lastClick = 0;

volatile uint8_t LED_state = 0;

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

	/* Retarget stdio to a text display. */
	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
		while (1)
			;
	}

	// Clear LED to see that program did not fail
	GPIO_PinOutClear(gpioPortC, 5);
	setup_effects();

	LCD_InitialRender();
	SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);

	while (1) {
		uint32_t input = (~GPIO_PortInGet(BUTTON_PORT)) & 0b111111;
		if (!input || msTicks - lastClick < 30)
			continue;

		if (button_pressed(input, BUTTON_NAVIGATION_UP)) {
			LCD_NavigateUp();
		} else if (button_pressed(input, BUTTON_NAVIGATION_DOWN)) {
			LCD_NavigateDown();
		} else if (button_pressed(input, BUTTON_NAVIGATION_LEFT)) {
			LCD_NavigateIn();
		} else if (button_pressed(input, BUTTON_NAVIGATION_RIGHT)) {
			LCD_NavigateOut();
		} else if (button_pressed(input, BUTTON_INCREMENT)) {
			LCD_IncrementValue();
		} else if (button_pressed(input, BUTTON_DECREMENT)) {
			LCD_DecrementValue();
		}
		while (((~GPIO_PortInGet(BUTTON_PORT)) & 0b111111) == input)
			;
		lastClick = msTicks;
	}
}
