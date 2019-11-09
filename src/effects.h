#ifndef SRC_EFFECTS_H_
#define SRC_EFFECTS_H_

#include "main.h"

#define EFFECTS 2

extern effect_t effects[2];

setting_t *create_setting(uint8_t id, char *name, char *unit, uint16_t value,
		uint16_t step_size, uint16_t min, uint16_t max);
effect_t create_effect(char *name, uint8_t n_settings);
void setup_effects();

#endif /* SRC_EFFECTS_H_ */
