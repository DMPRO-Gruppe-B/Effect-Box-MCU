#include "lcd.h"

int currentEffect = 0;
int currentSetting = -1;
int isInline = false;

unsigned int cursor = 0;
int effectIndices[EFFECTS];

uint8_t countEffectsAndSettings() {
	return n_settings + EFFECTS;
}

void calculateEffectIndices() {
	int index = 0;
	for (int i = 0; i < EFFECTS; i++) {
		effectIndices[i] = index;
		index += effects[i].n_settings + 1;
	}
}

bool isEffectIndex() {
	for (int i = 0; i < EFFECTS; i++) {
		if (effectIndices[i] == cursor)
			return true;
	}
	return false;
}

void LCD_InitialRender() {
	calculateEffectIndices();

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
	if (!isInline) {
		uint8_t effectsAndSettings = countEffectsAndSettings();
		printf(NO_CURSOR);
		if (cursor) {
			uint8_t stride = effects[currentEffect - 1].n_settings + 1;
			for (uint8_t i = 0; i <= stride; i++) {
				printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			}
			cursor -= stride;
			currentEffect -= 1;
		} else {
			uint8_t lastEffectIndex = effectsAndSettings - effects[EFFECTS - 1].n_settings - 1;
			cursor = lastEffectIndex;
			for (unsigned int i = 1; i < lastEffectIndex; i++) {
				printf(TEXTDISPLAY_ESC_SEQ_CURSOR_DOWN_ONE_LINE);
			}
			currentEffect = EFFECTS - 1;
		}
		printf(">\n");
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		GPIO_PinOutToggle(gpioPortF, 5);
	}
	// If cursor is inline
	else {
		// Navigate up should select previous setting if one exist
		// Check if current setting is not first setting
		if (currentSetting) {
			printf(NO_INLINE_CURSOR);
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			printf(INLINE_CURSOR);
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			cursor--;
			currentSetting--;
		}
		// Current setting is first setting: Should wrap around to last setting
		else {
			printf(NO_INLINE_CURSOR);
			cursor++;
			uint8_t n_settings = effects[currentEffect].n_settings;
			for (int i = 0; i < n_settings - 2; i++) {
				printf(TEXTDISPLAY_ESC_SEQ_CURSOR_DOWN_ONE_LINE);
				cursor++;
			}
			printf(INLINE_CURSOR);
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			currentSetting = n_settings - 1;
		}

	}
}

void LCD_NavigateDown() {
	if (!isInline) {
		uint8_t effectsAndSettings = countEffectsAndSettings();
		printf(NO_CURSOR);
		// Handles case if last effect selected
		if (cursor == effectsAndSettings - effects[EFFECTS - 1].n_settings - 1) {
			cursor = 0;
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_HOME_VT100);
			currentEffect = 0;
		} else {
			for (uint8_t i = 0; i < effects[cursor].n_settings; i++) {
				printf(TEXTDISPLAY_ESC_SEQ_CURSOR_DOWN_ONE_LINE);
			}
			cursor += effects[cursor].n_settings + 1;
			currentEffect++;
		}
		printf(CURSOR);
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		GPIO_PinOutToggle(LED_PORT, LED_LEFT);
	} else {
		uint8_t n_settings = effects[currentEffect].n_settings;
		// Current setting is last setting: Should wrap around to first setting
		if (currentSetting == n_settings - 1) {
			printf(NO_INLINE_CURSOR);
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			for (int i = 0; i < n_settings - 1; i++) {
				printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
				cursor++;
			}
			printf(INLINE_CURSOR);
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			currentSetting = 0;
		}
		// If current setting is not last setting
		else {
			printf(NO_INLINE_CURSOR);
			printf(INLINE_CURSOR);
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			cursor++;
			currentSetting++;
		}
	}
}

void LCD_NavigateIn() {
// Should only be able to navigate in if effect is selected
	if (isEffectIndex()) {
		printf(NO_CURSOR);
		printf(INLINE_CURSOR);
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		cursor++;
		isInline = true;
		currentSetting = 0;
	}
}

void LCD_NavigateOut() {
	if (!isEffectIndex()) {
		printf(NO_INLINE_CURSOR);
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		for (uint8_t i = 0; i < currentSetting + 1; i++) {
			printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
			cursor--;
		}
		currentSetting = -1;
		isInline = false;
		printf(CURSOR);
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
	}
}

void LCD_IncrementValue() {
	if (!isEffectIndex()) {
		setting_t *setting = effects[currentEffect].settings[currentSetting];
		if (setting->value == setting->max) {
			setting->value = setting->min;
		} else {
			setting->value += setting->step_size;
		}
		printf(INLINE_CURSOR_LINE, setting->name, setting->value,
				setting->unit);
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		send_setting(setting);
		GPIO_PinOutToggle(LED_PORT, LED_RIGHT);
	}
}

void LCD_DecrementValue() {
	if (!isEffectIndex()) {
		setting_t *setting = effects[currentEffect].settings[currentSetting];
		if (setting->value == setting->min) {
			setting->value = setting->max;
		} else {
			setting->value -= setting->step_size;
		}
		printf(INLINE_CURSOR_LINE, setting->name, setting->value,
				setting->unit);
		printf(TEXTDISPLAY_ESC_SEQ_CURSOR_UP_ONE_LINE);
		send_setting(setting);
		GPIO_PinOutToggle(LED_PORT, LED_RIGHT);
	}
}
