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

    float duty_cycle_u{0.0f};
    float duty_cycle_v{0.0f};
    float duty_cycle_w{0.0f};

    float average_dc_link_voltage{0.0f};
    float dc_link_voltage_1{0.0f};
    float dc_link_voltage_2{0.0f};
    float dc_link_voltage_3{0.0f};
    float dc_link_voltage_4{0.0f};

   private:
    float requested_duty_cycle_u{0.0};
    float requested_duty_cycle_v{0.0};
    float requested_duty_cycle_w{0.0};

    uint32_t requested_commutation_frequency_hz{0};
    uint32_t requested_dead_time_ns{0};

    float requested_modulation_index{0.0};
    float requested_modulation_frequency_hz{0.0};

    float requested_dc_link_voltage{0.0};

    uint8_t spi_id{0};

    SPIStackOrder *state_order;

    SPIStackOrder *start_test_pwm_order;

    SPIStackOrder *stop_control_order;
    SPIStackOrder *configure_commutation_parameters_order;

    SPIStackOrder *control_parameters_order;

    SPIStackOrder *start_space_vector_order;

    SPIStackOrder *fix_dc_link_voltage_order;
    SPIStackOrder *unfix_dc_link_voltage_order;
    SPIStackOrder *dc_link_order;

   public:
    SPI(StateMachine::state_id *master_general_state,
        StateMachine::state_id *master_nested_state);

    SPI(Pin &spi_ready_slave_pin, StateMachine::state_id *master_general_state,
        StateMachine::state_id *master_nested_state);

    void start();

    void update();

    void transmit_state();
    void request_control_parameters();
    void request_dc_link_voltage();

    void start_test_pwm(float duty_cycle_u, float duty_cycle_v,
                        float duty_cycle_w);

    void configure_commutation_parameters(uint32_t frequency_hz,
                                          uint32_t dead_time_ns);

    void stop_control();

    void start_space_vector(float modulation_index,
                            float modulation_frequency_hz);

    void fix_dc_link_voltage(float voltage);
    void unfix_dc_link_voltage();
};

};  // namespace BCU::Communication