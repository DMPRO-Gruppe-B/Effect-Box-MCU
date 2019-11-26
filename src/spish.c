#include "spish.h"
#include "systime.h"

void send_byte(byte b) {
	for (int j = 0; j < 8; j++) {
		if (b & BIT(7 - j)) {
			GPIO_PinOutSet(FPGA_SPI_PORT, FPGA_SPI_MOSI_PIN);
		} else {
			GPIO_PinOutClear(FPGA_SPI_PORT, FPGA_SPI_MOSI_PIN);
		}
		Delay(1);
		GPIO_PinOutSet(FPGA_SPI_PORT, FPGA_SPI_CLK_PIN);
		Delay(1);
		GPIO_PinOutClear(FPGA_SPI_PORT, FPGA_SPI_CLK_PIN);
	}
}

void send_setting(setting_t *setting) {
	static uint8_t data_length = 4;
	uint8_t data[data_length];
	data[0] = setting->id;
	if (setting->map_setting_value) {
		uint16_t value = setting->map_setting_value(setting);
		data[1] = (value & 0xFF00) >> 8;
		data[2] = value & 0xFF;
	} else {
		data[1] = (setting->value & 0xFF00) >> 8;
		data[2] = setting->value & 0xFF;
	}

	// Checksum
	data[3] = 0;
	for (uint8_t i = 0; i < data_length - 1; i++) {
		// Ignore overflow
		data[3] = (data[3] + (uint16_t) data[i]) & 0xFF;
	}
	data[3] = ~data[3];

	for (int i = 0; i < 4; i++) {
		GPIO_PinOutClear(FPGA_SPI_PORT, FPGA_SPI_CS_PIN); // Chip select
		Delay(1);
		for (uint8_t i = 0; i < data_length; i++)
			send_byte(data[i]);
		Delay(1);
		GPIO_PinOutSet(FPGA_SPI_PORT, FPGA_SPI_CS_PIN);
		GPIO_PinOutClear(FPGA_SPI_PORT, FPGA_SPI_MOSI_PIN);
		Delay(1);
	}
}
