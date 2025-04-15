#pragma once

#include "BCU/Communication/SPI.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication {

class CAN {
   public:
    uint16_t requested_duty_cycle_u{0};
    uint16_t requested_duty_cycle_v{0};
    uint16_t requested_duty_cycle_w{0};
    uint32_t requested_commutation_frequency_hz{0};
    uint32_t requested_dead_time_ns{0};
    float requested_modulation_index{0.0f};
    float requested_modulation_frequency_hz{0.0f};
    float requested_dc_link_voltage{0.0f};

    bool has_received_start_test_pwm{false};
    bool has_received_configure_commutation_parameters{false};
    bool has_received_stop_control{false};
    bool has_received_start_space_vector{false};
    bool has_received_fix_dc_link_voltage{false};
    bool has_received_unfix_dc_link_voltage{false};

   private:
    constexpr static uint16_t state_id{0x721};
    constexpr static uint16_t start_test_pwm_id{0x722};
    constexpr static uint16_t configure_commutation_parameters_id{0x723};
    constexpr static uint16_t stop_control_id{0x724};
    constexpr static uint16_t control_parameters_id{0x725};
    constexpr static uint16_t start_space_vector_id{0x726};
    constexpr static uint16_t fix_dc_link_voltage_id{0x727};
    constexpr static uint16_t unfix_dc_link_voltage_id{0x728};
    constexpr static uint16_t dc_link_id{0x729};

    uint8_t can_id;

    FDCAN::Packet last_read{};

   public:
    CAN();

    void start();
    void update();

    void transmit_state(StateMachine::state_id master_general_state,
                        StateMachine::state_id master_nested_state,
                        StateMachine::state_id slave_general_state,
                        StateMachine::state_id slave_nested_state);

    void transmit_control_parameters(float duty_cycle_u, float duty_cycle_v,
                                     float duty_cycle_w);

    void transmit_dc_link_voltage(float average_dc_link_voltage,
                                  float dc_link_voltage_1,
                                  float dc_link_voltage_2,
                                  float dc_link_voltage_3,
                                  float dc_link_voltage_4);
};

};  // namespace BCU::Communication