#pragma once

#include "ST-LIB.hpp"
#include "Shared/SPI.hpp"
#include "Shared/StateMachine.hpp"

namespace BCU::Communication {

class SPI {
   public:
    StateMachine::state_id slave_general_state{
        Shared::State::SharedStateMachine::GeneralState::Connecting};
    StateMachine::state_id slave_nested_state{
        Shared::State::SharedStateMachine::NestedState::Idle};

    float velocity_reference{0.0};
    float velocity_error{0.0};
    float u_current_measurement{0.0};
    float v_current_measurement{0.0};
    float w_current_measurement{0.0};
    float electrical_angle{0.0};
    float d_current_reference{0.0};
    float d_current_measurement{0.0};
    float d_current_error{0.0};
    float q_current_reference{0.0};
    float q_current_measurement{0.0};
    float q_current_error{0.0};
    float three_phase_unbalance{0.0};
    float d_target_voltage{0.0};
    float q_target_voltage{0.0};
    float u_target_voltage{0.0};
    float v_target_voltage{0.0};
    float w_target_voltage{0.0};
    float u_output_voltage{0.0};
    float v_output_voltage{0.0};
    float w_output_voltage{0.0};
    float u_duty_cycle{0.0};
    float v_duty_cycle{0.0};
    float w_duty_cycle{0.0};
    float angular_velocity{0.0};
    std::array<float, 3> position{{0.0, 0.0, 0.0}};
    std::array<float, 3> velocity{{0.0, 0.0, 0.0}};
    std::array<float, 3> acceleration{{0.0, 0.0, 0.0}};
    std::array<Shared::Communication::Direction, 3> direction{
        {Shared::Communication::Direction::FORWARD,
         Shared::Communication::Direction::FORWARD,
         Shared::Communication::Direction::FORWARD}};
    float average_position{0.0};
    float max_velocity{0.0};
    bool is_detecting{false};
    std::array<float, 4> dc_link_voltage{{0.0f, 0.0f, 0.0f, 0.0f}};
    std::array<std::array<float, 3>, 4> motor_phase_current{
        {{{0.0f, 0.0f, 0.0f}},
         {{0.0f, 0.0f, 0.0f}},
         {{0.0f, 0.0f, 0.0f}},
         {{0.0f, 0.0f, 0.0f}}}};
    std::array<PinState, 4> gate_driver_fault{
        {PinState::OFF, PinState::OFF, PinState::OFF, PinState::OFF}};
    std::array<PinState, 4> gate_driver_ready{
        {PinState::OFF, PinState::OFF, PinState::OFF, PinState::OFF}};

   private:
    float requested_velocity_reference{0.0};
    float requested_d_current_reference{0.0};
    float requested_q_current_reference{0.0};
    float requested_angular_velocity{0.0};
    float requested_duty_cycle_u{0.0};
    float requested_duty_cycle_v{0.0};
    float requested_duty_cycle_w{0.0};
    float requested_dc_link_voltage{0.0f};
    uint32_t requested_commutation_frequency_hz{0};
    uint32_t requested_commutation_dead_time_ns{0};

    uint8_t spi_id{0};

    SPIStackOrder *state_order;

    SPIStackOrder *control_parameters_order;

    SPIStackOrder *start_velocity_control_order;
    SPIStackOrder *start_current_control_order;
    SPIStackOrder *start_emulated_movement_order;
    SPIStackOrder *start_test_pwm_order;

    SPIStackOrder *stop_control_order;

    SPIStackOrder *enable_booster_order;

    SPIStackOrder *encoder_order;

    SPIStackOrder *force_dc_link_order;
    SPIStackOrder *unlock_dc_link_order;

    SPIStackOrder *commutation_settings_order;

    SPIStackOrder *motor_driver_order;

   public:
    SPI(StateMachine::state_id *master_general_state,
        StateMachine::state_id *master_nested_state);

    SPI(Pin &spi_ready_slave_pin, StateMachine::state_id *master_general_state,
        StateMachine::state_id *master_nested_state);

    void start();

    void update();

    void transmit_state();

    void enable_booster();

    void start_velocity_control(float velocity_reference);
    void start_current_control(float d_current_reference,
                               float q_current_reference);
    void start_emulated_movement(float d_current_reference,
                                 float q_current_reference,
                                 float angular_velocity);
    void start_test_pwm(float duty_cycle_u, float duty_cycle_v,
                        float duty_cycle_w);

    void stop_control();

    void force_dc_link(float dc_link_voltage);
    void unlock_dc_link();

    void change_commutation_settings(uint32_t frequency_hz,
                                     uint32_t dead_time_ns);

    void read_motor_drivers();
    void read_position_encoder();
    void read_control_parameters();
};

};  // namespace BCU::Communication