#pragma once

#include "ST-LIB.hpp"

namespace BCU::Pinout {

extern Pin& uart_tx_pin;
extern Pin& uart_rx_pin;

extern Pin& spi_chip_select_pin;
extern Pin& spi_mosi_pin;
extern Pin& spi_miso_pin;
extern Pin& spi_clock_pin;
extern Pin& spi_ready_slave_pin;

extern Pin& can_tx_pin;
extern Pin& can_rx_pin;
extern Pin& can_silent_mode_pin;

extern Pin& led_operational_pin;
extern Pin& led_fault_pin;
extern Pin& led_can_pin;
extern Pin& led_flash_pin;
extern Pin& led_sleep_pin;

extern Pin& buffer_1_enable_pin;
extern Pin& buffer_2_enable_pin;
extern Pin& buffer_3_enable_pin;

extern Pin& inverter_1_temp_pin;
extern Pin& inverter_2_temp_pin;
extern Pin& inverter_3_temp_pin;
extern Pin& inverter_4_temp_pin;

};  // namespace BCU::Pinout