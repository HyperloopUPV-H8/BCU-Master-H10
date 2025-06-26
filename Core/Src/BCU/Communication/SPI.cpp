#include "BCU/Communication/SPI.hpp"

namespace BCU::Communication {

SPI::SPI(::SPI::Peripheral &peripheral, StateMachine::state_id *general_state,
         StateMachine::state_id *operational_state)
    : spi_id(::SPI::inscribe(peripheral)),
      state_order(Shared::Communication::create_state_order(
          general_state, operational_state, &slave_general_state,
          &slave_operational_state)),
      encoder_order(Shared::Communication::create_encoder_order(
          &speetec_position, &speetec_velocity, &speetec_acceleration,
          &speetec_direction)),
      detailed_encoder_order(
          Shared::Communication::create_detailed_encoder_order(
              &speetec_1_position, &speetec_1_velocity, &speetec_1_acceleration,
              &speetec_direction_1, &speetec_2_position, &speetec_2_velocity,
              &speetec_2_acceleration, &speetec_direction_2)),
      test_pwm_order(Shared::Communication::create_test_pwm_order(
          &requested_u, &requested_v, &requested_w)),
      test_space_vector_order(
          Shared::Communication::create_test_space_vector_order(
              &requested_i_mod, &requested_mod_freq)),
      enable_current_control_order(
          Shared::Communication::create_enable_current_control_order(
              &requested_current_d, &requested_current_q)),
      enable_speed_control_order(
          Shared::Communication::create_enable_velocity_control_order(
              &requested_speed)) {}

void SPI::update() { ::SPI::Order_update(); }

bool SPI::sync_state() {
    return ::SPI::master_transmit_Order(spi_id, state_order);
}

bool SPI::sync_encoder() {
    return ::SPI::master_transmit_Order(spi_id, encoder_order) &&
           ::SPI::master_transmit_Order(spi_id, detailed_encoder_order);
}

bool SPI::send_test_pwm(float duty_u, float duty_v, float duty_w) {
    requested_u = duty_u;
    requested_v = duty_v;
    requested_w = duty_w;
    return ::SPI::master_transmit_Order(spi_id, test_pwm_order);
}

bool SPI::send_test_space_vector(float i_mod, float mod_freq) {
    requested_i_mod = i_mod;
    requested_mod_freq = mod_freq;
    return ::SPI::master_transmit_Order(spi_id, test_space_vector_order);
}

bool SPI::send_enable_current_control(float current) {
    requested_current_d = 0.0f;
    requested_current_q = current;
    return ::SPI::master_transmit_Order(spi_id, enable_current_control_order);
}

bool SPI::send_enable_speed_control(float speed) {
    requested_speed = speed;
    return ::SPI::master_transmit_Order(spi_id, enable_speed_control_order);
}

};  // namespace BCU::Communication
