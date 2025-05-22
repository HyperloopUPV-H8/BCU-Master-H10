#pragma once

#include "ST-LIB.hpp"
#include "Shared/SPI.hpp"

namespace BCU::Communication {

class SPI {
   public:
    StateMachine::state_id slave_general_state{0};
    StateMachine::state_id slave_operational_state{0};

    float duty_cycle_u{0.0f};
    float duty_cycle_v{0.0f};
    float duty_cycle_w{0.0f};
    uint32_t commutation_frequency_hz{0};
    uint16_t dead_time_ns{0};

    float measured_current_d{0.0f};
    float measured_current_q{0.0f};
    float measured_current_alpha{0.0f};
    float measured_current_beta{0.0f};
    float measured_current_u{0.0f};
    float measured_current_v{0.0f};
    float measured_current_w{0.0f};
    float electrical_angle{0.0f};

    float target_voltage_d{0.0f};
    float target_voltage_q{0.0f};
    float target_voltage_alpha{0.0f};
    float target_voltage_beta{0.0f};
    float target_voltage_u{0.0f};
    float target_voltage_v{0.0f};
    float target_voltage_w{0.0f};
    float dc_link_voltage{0.0f};

    float reference_current_d{0.0f};
    float reference_current_q{0.0f};
    float reference_speed{0.0f};
    float error_current_d{0.0f};
    float error_current_q{0.0f};
    float error_speed{0.0f};

    float measured_current_u_1{0.0f};
    float measured_current_v_1{0.0f};
    float measured_current_w_1{0.0f};
    float measured_current_u_2{0.0f};
    float measured_current_v_2{0.0f};
    float measured_current_w_2{0.0f};
    float measured_current_u_3{0.0f};
    float measured_current_v_3{0.0f};
    float measured_current_w_3{0.0f};
    float measured_current_u_4{0.0f};
    float measured_current_v_4{0.0f};
    float measured_current_w_4{0.0f};

    float speetec_1_position{0.0f};
    float speetec_1_velocity{0.0f};
    float speetec_1_acceleration{0.0f};
    float speetec_2_position{0.0f};
    float speetec_2_velocity{0.0f};
    float speetec_2_acceleration{0.0f};
    float speetec_3_position{0.0f};
    float speetec_3_velocity{0.0f};
    float speetec_3_acceleration{0.0f};
    float speetec_position{0.0f};
    float speetec_velocity{0.0f};
    float speetec_acceleration{0.0f};

    float driver_1_dc_link_voltage{0.0f};
    bool driver_1_ready{false};
    bool driver_1_fault{false};
    float driver_2_dc_link_voltage{0.0f};
    bool driver_2_ready{false};
    bool driver_2_fault{false};
    float driver_3_dc_link_voltage{0.0f};
    bool driver_3_ready{false};
    bool driver_3_fault{false};
    float driver_4_dc_link_voltage{0.0f};
    bool driver_4_ready{false};
    bool driver_4_fault{false};

    float motor_1_temperature{0.0f};
    float motor_2_temperature{0.0f};
    float motor_3_temperature{0.0f};
    float motor_4_temperature{0.0f};
    float driver_1_temperature{0.0f};
    float driver_2_temperature{0.0f};
    float driver_3_temperature{0.0f};
    float driver_4_temperature{0.0f};

   private:
    uint8_t spi_id;

    SPIStackOrder *state_order;

   public:
    SPI(::SPI::Peripheral &peripheral, StateMachine::state_id *general_state,
        StateMachine::state_id *operational_state);

    void update();

    bool sync_state();
};

};  // namespace BCU::Communication
