#include "effects.h"
#include "spish.h"

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
	setting->map_setting_value = NULL;

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

uint16_t map_div10(setting_t *setting) {
	return setting->value / 10;
}

uint16_t map_tremolo_rate(setting_t *setting) {
	return (setting->value * 32)/360;
}

void setup_effects() {
	effect_t delay = create_effect("Delay", 4);
	delay.settings[0] = create_setting(3, "Bypass", "", 1, 1, 0, 1);
	delay.settings[1] = create_setting(8, "Mix", "%", 50, 10, 0, 100);
	delay.settings[1]->map_setting_value = map_div10;
	delay.settings[2] = create_setting(7, "Feedback", "%", 50, 10, 0, 100);
	delay.settings[2]->map_setting_value = map_div10;
	delay.settings[3] = create_setting(4, "Delay", "ms", 900, 50, 50, 900);

	effect_t tremolo = create_effect("Tremolo", 4);
	tremolo.settings[0] = create_setting(5, "Bypass", "", 1, 1, 0, 1);
	tremolo.settings[1] = create_setting(9, "Depth", "%", 50, 10, 0, 100);
	tremolo.settings[1]->map_setting_value = map_div10;
	tremolo.settings[2] = create_setting(6, "Rate", "ms", 225, 45, 90, 540);
	tremolo.settings[2]->map_setting_value = map_tremolo_rate;
	tremolo.settings[3] = create_setting(14, "Wave Form", "", 0, 1, 0, 3);

	effect_t bitcrush = create_effect("Bitcrush & Dist.", 4);
	bitcrush.settings[0] = create_setting(0, "Bypass", "", 1, 1, 0, 1);
	bitcrush.settings[1] = create_setting(13, "Distortion", "bits", 0, 1, 0, 15);
	bitcrush.settings[2] = create_setting(1, "Depth", "bits", 0, 1, 0, 15);
	bitcrush.settings[3] = create_setting(2, "Rate", "div", 1, 2, 1, 63);

	effects[0] = bitcrush;
	effects[1] = tremolo;
	effects[2] = delay;
}
