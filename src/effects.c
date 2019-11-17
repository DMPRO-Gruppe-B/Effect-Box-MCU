#include "effects.h"

effect_t effects[EFFECTS];
uint8_t n_settings = 0;

setting_t *create_setting(uint8_t id, char *name, char *unit, uint16_t value, uint16_t step_size, uint16_t min,
		uint16_t max) {
	setting_t *setting = malloc(sizeof(setting_t));

	setting->name = malloc(strlen(name) + 1);
	setting->unit = malloc(strlen(unit) + 1);
	setting->id = id;
	setting->value = value;
	setting->step_size = step_size;
	setting->min = min;
	setting->max = max;

	strcpy(setting->name, name);
	strcpy(setting->unit, unit);

	n_settings++;
	return setting;
}

effect_t create_effect(char *name, uint8_t n_extra_settings) {
	effect_t e;
	uint8_t n_settings = n_extra_settings + 1;
	e.n_settings = n_settings;
	e.name = malloc(strlen(name) + 1);
	strcpy(e.name, name);
	e.settings = malloc(n_settings * sizeof(setting_t *));

	// Add implicit enable setting
	e.settings[ENABLE_SETTING_ID] = create_setting(0b00000000, "Enable", "", 0, 1, 0, 1);

	return e;
}

void setup_effects() {
	// Bitcrusher
	effect_t bitcrusher = create_effect("Bitcrusher", 2);
	bitcrusher.settings[1] = create_setting(0b00000001, "Bits", "bits", 0, 1, 0, 15);
	bitcrusher.settings[2] = create_setting(0b00000010, "Rate", "x2", 0, 1, 0, 15);

	effect_t delay = create_effect("Delay", 1);
	delay.settings[1] = create_setting(0b00000101, "Delay", "ms", 0, 50, 0, 500);

	effects[0] = bitcrusher;
	effects[1] = delay;
}

void update_effect_led() {
	uint8_t anyEnabled = 0;
	for (uint8_t i = 0; i < EFFECTS; i++) {
		if (effects[i].settings[ENABLE_SETTING_ID]->value) {
			anyEnabled = 1;
			break;
		}
	}

	if (anyEnabled) {
		GPIO_PinOutSet(LED_PORT, LED_RIGHT);
	} else {
		GPIO_PinOutClear(LED_PORT, LED_RIGHT);
	}
}
