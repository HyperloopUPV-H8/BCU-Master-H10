#include "BCU/Communication/SPI.hpp"

namespace BCU::Communication {

SPI::SPI(StateMachine::state_id *master_general_state,
         StateMachine::state_id *master_nested_state)
    : spi_id(::SPI::inscribe(::SPI::spi3)),
      state_order(Shared::Communication::create_state_order(
          master_general_state, master_nested_state, &slave_general_state,
          &slave_nested_state)),
      control_parameters_order(
          Shared::Communication::create_control_parameters_order(
              &velocity_reference, &velocity_error, &u_current_measurement,
              &v_current_measurement, &w_current_measurement, &electrical_angle,
              &d_current_reference, &d_current_measurement, &d_current_error,
              &q_current_reference, &q_current_measurement, &q_current_error,
              &three_phase_unbalance, &d_target_voltage, &q_target_voltage,
              &u_target_voltage, &v_target_voltage, &w_target_voltage,
              &u_output_voltage, &v_output_voltage, &w_output_voltage,
              &u_duty_cycle, &v_duty_cycle, &w_duty_cycle, &angular_velocity)),
      start_velocity_control_order(
          Shared::Communication::create_start_velocity_control_order(
              &requested_velocity_reference)),
      start_current_control_order(
          Shared::Communication::create_start_current_control_order(
              &requested_d_current_reference, &requested_q_current_reference)),
      start_emulated_movement_order(
          Shared::Communication::create_start_emulated_movement_order(
              &requested_d_current_reference, &requested_q_current_reference,
              &requested_angular_velocity)),
      start_test_pwm_order(Shared::Communication::create_start_test_pwm_order(
          &requested_duty_cycle_u, &requested_duty_cycle_v,
          &requested_duty_cycle_w)),
      stop_control_order(Shared::Communication::create_stop_control_order()),
      enable_booster_order(
          Shared::Communication::create_enable_booster_order()),
      encoder_order(Shared::Communication::create_position_encoder_order(
          {&position[0], &position[1], &position[2]},
          {&velocity[0], &velocity[1], &velocity[2]},
          {&acceleration[0], &acceleration[1], &acceleration[2]},
          {&direction[0], &direction[1], &direction[2]}, &average_position,
          &max_velocity, &is_detecting)),
      force_dc_link_order(Shared::Communication::create_force_dc_link_order(
          &requested_dc_link_voltage)),
      unlock_dc_link_order(
          Shared::Communication::create_unlock_dc_link_order()),
      commutation_settings_order(
          Shared::Communication::create_commutation_settings_order(
              &requested_commutation_frequency_hz,
              &requested_commutation_dead_time_ns)),
      motor_driver_order(Shared::Communication::create_motor_driver_order(
          {&dc_link_voltage[0], &dc_link_voltage[1], &dc_link_voltage[2],
           &dc_link_voltage[3]},
          {std::array<float *, 3>{&motor_phase_current[0][0],
                                  &motor_phase_current[0][1],
                                  &motor_phase_current[0][2]},
           std::array<float *, 3>{&motor_phase_current[1][0],
                                  &motor_phase_current[1][1],
                                  &motor_phase_current[1][2]},
           std::array<float *, 3>{&motor_phase_current[2][0],
                                  &motor_phase_current[2][1],
                                  &motor_phase_current[2][2]},
           std::array<float *, 3>{&motor_phase_current[3][0],
                                  &motor_phase_current[3][1],
                                  &motor_phase_current[3][2]}},
          {&gate_driver_fault[0], &gate_driver_fault[1], &gate_driver_fault[2],
           &gate_driver_fault[3]},
          {&gate_driver_ready[0], &gate_driver_ready[1], &gate_driver_ready[2],
           &gate_driver_ready[3]})) {}

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

void SPI::enable_booster() {
    ::SPI::master_transmit_Order(spi_id, enable_booster_order);
}

void SPI::start_velocity_control(double velocity_reference) {
    requested_velocity_reference = velocity_reference;
    ::SPI::master_transmit_Order(spi_id, start_velocity_control_order);
}

void SPI::start_current_control(double d_current_reference,
                                double q_current_reference) {
    requested_d_current_reference = d_current_reference;
    requested_q_current_reference = q_current_reference;
    ::SPI::master_transmit_Order(spi_id, start_current_control_order);
}

void SPI::start_emulated_movement(double d_current_reference,
                                  double q_current_reference,
                                  double angular_velocity) {
    requested_d_current_reference = d_current_reference;
    requested_q_current_reference = q_current_reference;
    requested_angular_velocity = angular_velocity;
    ::SPI::master_transmit_Order(spi_id, start_emulated_movement_order);
}

void SPI::start_test_pwm(double duty_cycle_u, double duty_cycle_v,
                         double duty_cycle_w) {
    requested_duty_cycle_u = duty_cycle_u;
    requested_duty_cycle_v = duty_cycle_v;
    requested_duty_cycle_w = duty_cycle_w;
    ::SPI::master_transmit_Order(spi_id, start_test_pwm_order);
}

void SPI::stop_control() {
    ::SPI::master_transmit_Order(spi_id, stop_control_order);
}

void SPI::force_dc_link(float dc_link_voltage) {
    requested_dc_link_voltage = dc_link_voltage;
    ::SPI::master_transmit_Order(spi_id, force_dc_link_order);
}

void SPI::unlock_dc_link() {
    ::SPI::master_transmit_Order(spi_id, unlock_dc_link_order);
}

void SPI::change_commutation_settings(uint32_t frequency_hz,
                                      uint32_t dead_time_ns) {
    requested_commutation_frequency_hz = frequency_hz;
    requested_commutation_dead_time_ns = dead_time_ns;
    ::SPI::master_transmit_Order(spi_id, commutation_settings_order);
}

void SPI::read_motor_drivers() {
    ::SPI::master_transmit_Order(spi_id, motor_driver_order);
}

void SPI::read_control_parameters() {
    ::SPI::master_transmit_Order(spi_id, control_parameters_order);
}

void SPI::read_position_encoder() {
    ::SPI::master_transmit_Order(spi_id, encoder_order);
}

};  // namespace BCU::Communication
