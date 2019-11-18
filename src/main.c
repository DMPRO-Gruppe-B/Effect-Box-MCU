#include "main.h"

#include <core_cm3.h>
#include <em_gpio.h>
#include <stdbool.h>
#include <sys/_stdint.h>

#include "effects.h"
#include "lcd.h"
#include "systime.h"

/* Counts 1ms timeTicks */
volatile uint32_t lastClick = 0;

void GPIO_Init() {
	CMU_ClockEnable(cmuClock_GPIO, true);

	/* Setup LEDs */
	GPIO_PinModeSet(LED_PORT, LED_LEFT, gpioModePushPull, 0);
	GPIO_PinModeSet(LED_PORT, LED_RIGHT, gpioModePushPull, 0);

	/* Setup buttons */

	GPIO_PinModeSet(BUTTON_PORT, BUTTON_NAVIGATION_UP, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_NAVIGATION_DOWN, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_NAVIGATION_LEFT, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_NAVIGATION_RIGHT, gpioModeInput, 0);

	GPIO_PinModeSet(BUTTON_PORT, BUTTON_INCREMENT, gpioModeInput, 0);
	GPIO_PinModeSet(BUTTON_PORT, BUTTON_DECREMENT, gpioModeInput, 0);

	GPIO_PinModeSet(FPGA_SPI_PORT, FPGA_SPI_MOSI_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet(FPGA_SPI_PORT, FPGA_SPI_MISO_PIN, gpioModeInput, 0);
	GPIO_PinModeSet(FPGA_SPI_PORT, FPGA_SPI_CLK_PIN, gpioModePushPull, 0);
	GPIO_PinModeSet(FPGA_SPI_PORT, FPGA_SPI_CS_PIN, gpioModePushPull, 0);

	GPIO_PinModeSet(FPGA_SOFT_RESET_PORT, FPGA_SOFT_RESET_PIN, gpioModePushPull, 0);
}

void reset_fpga() {
	GPIO_PinOutSet(FPGA_SOFT_RESET_PORT, FPGA_SOFT_RESET_PIN);
	Delay(10);
	GPIO_PinOutClear(FPGA_SOFT_RESET_PORT, FPGA_SOFT_RESET_PIN);
	Delay(10);
}

int main(void) {
	/* Setup chip */
	CHIP_Init();
	GPIO_Init();

	/* Enable power LED */
	GPIO_PinOutSet(LED_PORT, LED_LEFT);

	/* Initialize HFXO with specific parameters */
	CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
	CMU_HFXOInit(&hfxoInit);

	/* Enable and set HFXO for HFCLK */
	CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
	CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

	/* Setup display */
	DISPLAY_Init();
	/* Retarget stdio to a text display. */
	if (RETARGET_TextDisplayInit() != TEXTDISPLAY_EMSTATUS_OK) {
		while (1);
	}
	LCD_SplashScreen();

	/* Load effects */
	setup_effects();

	/* Setup clock */
	SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000);

	/* Reset FPGA and send initial settings */
	reset_fpga();
	send_all_effects_to_fpga();

	/* Show menu */
	LCD_InitialRender();

	/* Enable ready LED */
	GPIO_PinOutSet(LED_PORT, LED_RIGHT);

	while (1) {
		uint32_t input = (~GPIO_PortInGet(BUTTON_PORT)) & 0b111111;
		if (!input || msTicks - lastClick < 100)
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
