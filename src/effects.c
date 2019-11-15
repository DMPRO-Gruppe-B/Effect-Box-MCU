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

effect_t create_effect(char *name, uint8_t n_settings) {
	effect_t e;
	e.n_settings = n_settings;
	e.name = malloc(strlen(name) + 1);
	strcpy(e.name, name);
	e.settings = malloc(n_settings * sizeof(setting_t *));
	return e;
}
void send_all_effects_to_fpga() {
	for (int i = 0; i < EFFECTS; i++) {
		effect_t effect = effects[i];
		for (int setting_index = 0; setting_index < effect.n_settings; setting_index++) {
			setting_t *setting = effect.settings[setting_index];
			send_setting(setting);
		}
	}
}

void setup_effects() {
	// Bitcrusher
	effect_t bitcrusher = create_effect("Bitcrusher", 3);
	bitcrusher.settings[0] = create_setting(0, "Bypass", "", 1, 1, 0, 1);
	bitcrusher.settings[1] = create_setting(1, "Bits", "bits", 0, 1, 0, 10);
	bitcrusher.settings[2] = create_setting(2, "Sample int", "", 1, 1, 1, 16);

	/*
	effect_t delay = create_effect("Delay", 2);
	delay.settings[0] = create_setting(0b00000100, "Bypass", "", 1, 1, 0, 1);
	delay.settings[1] = create_setting(0b00000101, "Delay", "ms", 0, 50, 0, 500);
	*/

	effect_t tremolo = create_effect("Tremolo", 2);
	tremolo.settings[0] = create_setting(3, "Bypass", "", 1, 1, 0, 1);
	tremolo.settings[1] = create_setting(4, "SinMult", "", 18, 1, 8, 40);

	effects[0] = bitcrusher;
	//effects[1] = delay;
	effects[1] = tremolo;
}
