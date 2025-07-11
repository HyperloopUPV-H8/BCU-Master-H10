#pragma once

#include "ST-LIB.hpp"
#include "Shared/SPI.hpp"

namespace BCU::Communication {

class SPI {
   public:
    StateMachine::state_id slave_general_state{0};
    StateMachine::state_id slave_operational_state{0};

    double duty_cycle_u{0.0f};
    double duty_cycle_v{0.0f};
    double duty_cycle_w{0.0f};
    uint32_t commutation_frequency_hz{0};
    uint16_t dead_time_ns{0};

    double measured_current_d{0.0f};
    double measured_current_q{0.0f};
    double measured_current_alpha{0.0f};
    double measured_current_beta{0.0f};
    double measured_current_u{0.0f};
    double measured_current_v{0.0f};
    double measured_current_w{0.0f};
    double electrical_angle{0.0f};

    double target_voltage_d{0.0f};
    double target_voltage_q{0.0f};
    double target_voltage_alpha{0.0f};
    double target_voltage_beta{0.0f};
    double target_voltage_u{0.0f};
    double target_voltage_v{0.0f};
    double target_voltage_w{0.0f};
    double dc_link_voltage{0.0f};

    double reference_current_d{0.0f};
    double reference_current_q{0.0f};
    double reference_speed{0.0f};
    double error_current_d{0.0f};
    double error_current_q{0.0f};
    double error_speed{0.0f};

    double measured_current_u_1{0.0f};
    double measured_current_v_1{0.0f};
    double measured_current_w_1{0.0f};
    double measured_current_u_2{0.0f};
    double measured_current_v_2{0.0f};
    double measured_current_w_2{0.0f};
    double measured_current_u_3{0.0f};
    double measured_current_v_3{0.0f};
    double measured_current_w_3{0.0f};
    double measured_current_u_4{0.0f};
    double measured_current_v_4{0.0f};
    double measured_current_w_4{0.0f};

    double speetec_1_position{0.0f};
    double speetec_1_velocity{0.0f};
    double speetec_1_acceleration{0.0f};
    uint8_t speetec_direction_1{0};
    double speetec_2_position{0.0f};
    double speetec_2_velocity{0.0f};
    double speetec_2_acceleration{0.0f};
    uint8_t speetec_direction_2{0};
    double speetec_3_position{0.0f};
    double speetec_3_velocity{0.0f};
    double speetec_3_acceleration{0.0f};
    uint8_t speetec_direction_3{0};
    double speetec_position{0.0f};
    double speetec_velocity{0.0f};
    double speetec_acceleration{0.0f};
    uint8_t speetec_direction{0};

    double driver_1_dc_link_voltage{0.0f};
    bool driver_1_ready{false};
    bool driver_1_fault{false};
    double driver_2_dc_link_voltage{0.0f};
    bool driver_2_ready{false};
    bool driver_2_fault{false};
    double driver_3_dc_link_voltage{0.0f};
    bool driver_3_ready{false};
    bool driver_3_fault{false};
    double driver_4_dc_link_voltage{0.0f};
    bool driver_4_ready{false};
    bool driver_4_fault{false};

    double motor_1_temperature{0.0f};
    double motor_2_temperature{0.0f};
    double motor_3_temperature{0.0f};
    double motor_4_temperature{0.0f};
    double driver_1_temperature{0.0f};
    double driver_2_temperature{0.0f};
    double driver_3_temperature{0.0f};
    double driver_4_temperature{0.0f};

   private:
    uint8_t spi_id;

    SPIStackOrder *state_order;

    SPIStackOrder *encoder_order;
    SPIStackOrder *detailed_encoder_order;

    float requested_u{0.0f};
    float requested_v{0.0f};
    float requested_w{0.0f};

    SPIStackOrder *test_pwm_order;

    float requested_i_mod{0.0f};
    float requested_mod_freq{0.0f};

    SPIStackOrder *test_space_vector_order;

    float requested_current_d{0.0f};
    float requested_current_q{0.0f};

    SPIStackOrder *enable_current_control_order;

    float requested_speed{0.0f};

    SPIStackOrder *enable_speed_control_order;

   public:
    SPI(::SPI::Peripheral &peripheral, StateMachine::state_id *general_state,
        StateMachine::state_id *operational_state);

    void update();

    bool sync_state();
    bool sync_encoder();

    bool send_test_pwm(float duty_u, float duty_v, float duty_w);
    bool send_test_space_vector(float i_mod, float mod_freq);
    bool send_enable_current_control(float current_ref);
    bool send_enable_speed_control(float speed_ref);
};

};  // namespace BCU::Communication
