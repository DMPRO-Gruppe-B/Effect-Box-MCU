#include "lcd.h"
#include "main.h"
#include "effects.h"

//char *lcd_effects[] = { DELAY, COMPRESSOR, BITCRUSHER };
const char *units[] = { MILLISECONDS, PERCENTAGE, BITS };
int values[] = { 0, 0, 0 };
const int inc[] = { 10, 10, 1 };

unsigned int cursor = 0;

uint8_t countEffectsAndSettings() {
	uint8_t count = 0;
	for (int i = 0; i < EFFECTS; i++) {
		count += effects[i].n_settings;
	}
	return count + (uint8_t) EFFECTS;
}

void LCD_InitialRender() {
	printf(CURSOR_LINE, effects[0].name);
	for (uint8_t i = 0; i < effects[0].n_settings; i++) {
		setting_t *setting = effects[0].settings[i];
		printf(INLINE_LINE, setting->name, setting->value, setting->unit);
	}
	for (unsigned int j = 1; j < EFFECTS; j++) {
		printf(LINE, effects[j].name);
		for (uint8_t i = 0; i < effects[j].n_settings; i++) {
			setting_t *setting = effects[j].settings[i];
			printf(INLINE_LINE, setting->name, setting->value, setting->unit);
		}
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
	uint8_t effectsAndSettings = countEffectsAndSettings();
	// Handles case if last effect selected
	if (cursor == effectsAndSettings - effects[EFFECTS - 1].n_settings - 1) {
		cursor = 0;
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT100);
	}
	// Handles case for last setting in last effect (should wrap to first setting in last effect)
	else if (cursor >= countEffectsAndSettings()) {
		for (uint8_t i = 0; i < effects[cursor].n_settings; i++) {
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		}
		cursor -= effects[cursor].n_settings - 1;
	}
	else {
		for (uint8_t i = 0; i < effects[cursor].n_settings; i++) {
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_DOWN_ONE_LINE);
		}
		cursor += effects[cursor].n_settings + 1;
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
	printf(CURSOR_LINE, effects[cursor].name);
	printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
	GPIO_PinOutToggle(gpioPortC, 5);
}

void LCD_DecrementValue() {
	if (values[cursor] == 0) {
		values[cursor] = inc[cursor] * 10;
	} else {
		values[cursor] -= inc[cursor];
	}
	printf(CURSOR_LINE, effects[cursor].name);
	printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
	GPIO_PinOutToggle(gpioPortC, 5);
}
