#pragma once

#include "ST-LIB.hpp"

namespace BCU::Pinout {

extern Pin& uart_tx_pin;
extern Pin& uart_rx_pin;

extern Pin& spi_chip_select_pin;
extern Pin& spi_mosi_pin;
extern Pin& spi_miso_pin;
extern Pin& spi_clock_pin;

extern Pin& can_tx_pin;
extern Pin& can_rx_pin;
extern Pin& can_silent_mode_pin;

extern Pin& led_operational_pin;
extern Pin& led_fault_pin;
extern Pin& led_can_pin;
extern Pin& led_flash_pin;
extern Pin& led_sleep_pin;

extern Pin& buffer_enable_1_pin;
extern Pin& buffer_enable_2_pin;
extern Pin& buffer_enable_3_pin;

extern Pin& inverter_temp_1_pin;
extern Pin& inverter_temp_2_pin;
extern Pin& inverter_temp_3_pin;
extern Pin& inverter_temp_4_pin;

extern Pin& booster_temp_1_pin;
extern Pin& booster_temp_2_pin;
extern Pin& booster_temp_3_pin;
extern Pin& booster_temp_4_pin;

extern Pin& gpio_1_pin;
extern Pin& gpio_2_pin;
extern Pin& gpio_3_pin;
extern Pin& gpio_4_pin;

};  // namespace BCU::Pinout