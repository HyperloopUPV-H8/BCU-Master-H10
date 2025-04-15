#include "BCU/Communication/SPI.hpp"

namespace BCU::Communication {

SPI::SPI(StateMachine::state_id *master_general_state,
         StateMachine::state_id *master_nested_state)
    : spi_id(::SPI::inscribe(::SPI::spi3)),
      state_order(Shared::Communication::create_state_order(
          master_general_state, master_nested_state, &slave_general_state,
          &slave_nested_state)),
      start_test_pwm_order(Shared::Communication::create_start_test_pwm_order(
          &requested_duty_cycle_u, &requested_duty_cycle_v,
          &requested_duty_cycle_w)),
      stop_control_order(Shared::Communication::create_stop_control_order()),
      configure_commutation_parameters_order(
          Shared::Communication::create_configure_commutation_parameters_order(
              &requested_commutation_frequency_hz, &requested_dead_time_ns)),
      control_parameters_order(
          Shared::Communication::create_control_parameters_order(
              &duty_cycle_u, &duty_cycle_v, &duty_cycle_w)),
      start_space_vector_order(
          Shared::Communication::create_start_space_vector_order(
              &requested_modulation_index, &requested_modulation_frequency_hz)),
      fix_dc_link_voltage_order(
          Shared::Communication::create_fix_dc_link_voltage_order(
              &requested_dc_link_voltage)),
      unfix_dc_link_voltage_order(
          Shared::Communication::create_unfix_dc_link_voltage_order()),
      dc_link_order(Shared::Communication::create_dc_link_order(
          &average_dc_link_voltage, &dc_link_voltage_1, &dc_link_voltage_2,
          &dc_link_voltage_3, &dc_link_voltage_4)) {}

SPI::SPI(Pin &spi_ready_slave_pin, StateMachine::state_id *master_general_state,
         StateMachine::state_id *master_nested_state)
    : SPI(master_general_state, master_nested_state) {
    ::SPI::assign_RS(spi_id, spi_ready_slave_pin);
}

void SPI::start() {}

void SPI::update() { ::SPI::Order_update(); }

void SPI::transmit_state() {
    ::SPI::master_transmit_Order(spi_id, state_order);
}

void SPI::request_control_parameters() {
    ::SPI::master_transmit_Order(spi_id, control_parameters_order);
}

void SPI::request_dc_link_voltage() {
    ::SPI::master_transmit_Order(spi_id, dc_link_order);
}

void SPI::start_test_pwm(float duty_cycle_u, float duty_cycle_v,
                         float duty_cycle_w) {
    requested_duty_cycle_u = duty_cycle_u;
    requested_duty_cycle_v = duty_cycle_v;
    requested_duty_cycle_w = duty_cycle_w;
    ::SPI::master_transmit_Order(spi_id, start_test_pwm_order);
}

void SPI::configure_commutation_parameters(uint32_t frequency_hz,
                                           uint32_t dead_time_ns) {
    requested_commutation_frequency_hz = frequency_hz;
    requested_dead_time_ns = dead_time_ns;
    ::SPI::master_transmit_Order(spi_id,
                                 configure_commutation_parameters_order);
}

void SPI::stop_control() {
    ::SPI::master_transmit_Order(spi_id, stop_control_order);
}

void SPI::start_space_vector(float modulation_index,
                             float modulation_frequency_hz) {
    requested_modulation_index = modulation_index;
    requested_modulation_frequency_hz = modulation_frequency_hz;
    ::SPI::master_transmit_Order(spi_id, start_space_vector_order);
}

void SPI::fix_dc_link_voltage(float voltage) {
    requested_dc_link_voltage = voltage;
    ::SPI::master_transmit_Order(spi_id, fix_dc_link_voltage_order);
}

void SPI::unfix_dc_link_voltage() {
    ::SPI::master_transmit_Order(spi_id, unfix_dc_link_voltage_order);
}

};  // namespace BCU::Communication
