#include "spish.h"

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
	GPIO_PinOutClear(FPGA_SPI_PORT, FPGA_SPI_CS_PIN); // Chip select
	Delay(2);
	send_byte(setting->id);
	send_byte((byte) (setting->value & 0xFF00) >> 8);
	send_byte((byte) setting->value & 0xFF);
	Delay(8);
	GPIO_PinOutSet(FPGA_SPI_PORT, FPGA_SPI_CS_PIN);
	GPIO_PinOutClear(FPGA_SPI_PORT, FPGA_SPI_MOSI_PIN);
	Delay(2);
}
