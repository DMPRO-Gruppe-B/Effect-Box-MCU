#include "lcd.h"
#include "main.h"

char *lcd_effects[] = { DELAY, COMPRESSOR, BITCRUSHER };
const char *units[] = { MILLISECONDS, PERCENTAGE, BITS };
int values[] = { 0, 0, 0 };
const int inc[] = { 10, 10, 1 };

unsigned int cursor = 0;

void LCD_InitialRender() {
	printf(CURSOR_LINE, lcd_effects[0], values[0], units[0]);
	for (unsigned int i = 1; i < EFFECTS; i++) {
		printf(LINE, lcd_effects[i], values[i], units[i]);
	}
	printf(TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT100);
}

void LCD_NavigateUp() {
	printf(" \n");
	if (cursor) {
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		cursor--;
	} else {
		cursor = EFFECTS - 1;
		for (unsigned int i = 1; i < EFFECTS - 1; i++) {
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_DOWN_ONE_LINE);
		}
	}
	printf(">\n");
	printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
	GPIO_PinOutToggle(gpioPortF, 5);
}

void LCD_NavigateDown() {
	printf(NO_CURSOR);
	if (cursor < EFFECTS - 1) {
		cursor++;
	} else {
		cursor = 0;
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT100);
	}
	printf(CURSOR);
	printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
	GPIO_PinOutToggle(gpioPortC, 4);
}

void LCD_IncrementValue() {
	if (values[cursor] == inc[cursor] * 10) {
		values[cursor] = 0;
	} else {
		values[cursor] += inc[cursor];
	}
	printf(CURSOR_LINE, lcd_effects[cursor], values[cursor], units[cursor]);
	printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
	GPIO_PinOutToggle(gpioPortC, 5);
}

void LCD_DecrementValue() {
	if (values[cursor] == 0) {
		values[cursor] = inc[cursor] * 10;
	} else {
		values[cursor] -= inc[cursor];
	}
	printf(CURSOR_LINE, lcd_effects[cursor], values[cursor], units[cursor]);
	printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
	GPIO_PinOutToggle(gpioPortC, 5);
}
