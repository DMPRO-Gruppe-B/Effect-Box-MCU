#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"

#include "hal-config.h"

#include "textdisplay.h"
#include "retargettextdisplay.h"
#include "display.h"

#define BUTTON_PORT               (gpioPortA)
#define BUTTON_NAVIGATION_UP      (0)
#define BUTTON_NAVIGATION_DOWN    (2)
#define BUTTON_NAVIGATION_LEFT    (1)
#define BUTTON_NAVIGATION_RIGHT   (3)
#define BUTTON_INCREMENT          (4)
#define BUTTON_DECREMENT          (5)

#define LED_PORT                  (gpioPortC)
#define LED_LEFT                  (4)
#define LED_RIGHT                 (5)

#define FPGA_SPI_PORT             (FPGA_SPI_CS_PORT)

#define FPGA_SOFT_RESET_PORT      (gpioPortB)
#define FPGA_SOFT_RESET_PIN       (7U)

#define button_pressed(portInput, button) ((1 << button) & input)


#define BIT(x) (1U << (x))

typedef unsigned char byte;

typedef struct {
	uint8_t id;
	char *name;
	char *unit;
	uint16_t value;
	uint16_t step_size;
	uint16_t min;
	uint16_t max;
} setting_t;

typedef struct {
	char *name;
	uint8_t n_settings;
	setting_t **settings;
} effect_t;

#endif /* SRC_MAIN_H_ */
