#pragma once

#include "BCU/Actuators/LEDs.hpp"
#include "BCU/Actuators/MotorDriver.hpp"
#include "BCU/Communication/Ethernet.hpp"
#include "BCU/Communication/SPI.hpp"
#include "ST-LIB.hpp"
#include "Shared/StateMachine.hpp"

namespace BCU {

class ProtectionManagerHandle {
   public:
    ProtectionManagerHandle(StateMachine &state_machine,
                            StateMachine::state_id fault_state);
    void start();
    void update_high_frequency();
    void update_low_frequency();
};

class STLIBHandle {
   public:
    STLIBHandle(string mac = "00:80:e1:00:00:00", string ip = "192.168.1.4",
                string subnet_mask = "255.255.0.0",
                string gateway = "192.168.1.1",
                UART::Peripheral &printf_peripheral = UART::uart2);
    void update();
};

class Board {
    enum GeneralState : uint8_t {
        Connecting = 0,
        Operational = 1,
        Fault = 2,
    };

    enum OperationalState : uint8_t {
        Idle = 0,
        Precharge = 1,
    };

    volatile bool should_update_low_frequency{false};
    volatile bool should_send_data_1khz{false};
    volatile bool should_send_data_60hz{false};

    Shared::State::SharedStateMachine state_machine;

    ProtectionManagerHandle protection_manager{state_machine.general,
                                               GeneralState::Fault};

    Actuators::LEDs leds;

    Actuators::MotorDriver motor_driver;

    Communication::SPI spi{::SPI::spi1, &state_machine.general.current_state,
                           &state_machine.nested.current_state};

    Communication::Ethernet ethernet{
        {
            .duty_cycle_u{nullptr},
            .duty_cycle_v{nullptr},
            .duty_cycle_w{nullptr},
            .commutation_frequency_hz{nullptr},
            .dead_time_ns{nullptr},
        },
        {
            .measured_current_d{nullptr},
            .measured_current_q{nullptr},
            .measured_current_alpha{nullptr},
            .measured_current_beta{nullptr},
            .measured_current_u{nullptr},
            .measured_current_v{nullptr},
            .measured_current_w{nullptr},
            .electrical_angle{nullptr},
        },
        {
            .target_voltage_d{nullptr},
            .target_voltage_q{nullptr},
            .target_voltage_alpha{nullptr},
            .target_voltage_beta{nullptr},
            .target_voltage_u{nullptr},
            .target_voltage_v{nullptr},
            .target_voltage_w{nullptr},
            .dc_link_voltage{nullptr},
        },
        {
            .reference_current_d{nullptr},
            .reference_current_q{nullptr},
            .reference_speed{nullptr},
            .error_current_d{nullptr},
            .error_current_q{nullptr},
            .error_speed{nullptr},
        },
        {
            .measured_current_u_1{nullptr},
            .measured_current_v_1{nullptr},
            .measured_current_w_1{nullptr},
            .measured_current_u_2{nullptr},
            .measured_current_v_2{nullptr},
            .measured_current_w_2{nullptr},
        },
        {
            .speetec_1_position{nullptr},
            .speetec_1_velocity{nullptr},
            .speetec_1_acceleration{nullptr},
            .speetec_2_position{nullptr},
            .speetec_2_velocity{nullptr},
            .speetec_2_acceleration{nullptr},
            .speetec_3_position{nullptr},
            .speetec_3_velocity{nullptr},
            .speetec_3_acceleration{nullptr},
            .speetec_position{nullptr},
            .speetec_velocity{nullptr},
            .speetec_acceleration{nullptr},
        },
        {
            .driver_1_dc_link_voltage{nullptr},
            .driver_1_ready{nullptr},
            .driver_1_fault{nullptr},
            .driver_2_dc_link_voltage{nullptr},
            .driver_2_ready{nullptr},
            .driver_2_fault{nullptr},
            .driver_3_dc_link_voltage{nullptr},
            .driver_3_ready{nullptr},
            .driver_3_fault{nullptr},
            .driver_4_dc_link_voltage{nullptr},
            .driver_4_ready{nullptr},
            .driver_4_fault{nullptr},
        },
        {
            .driver_1_temperature{nullptr},
            .motor_1_temperature{nullptr},
            .driver_2_temperature{nullptr},
            .motor_2_temperature{nullptr},
            .driver_3_temperature{nullptr},
            .motor_3_temperature{nullptr},
            .driver_4_temperature{nullptr},
            .motor_4_temperature{nullptr},
        },
        {
            .master_general_state{&state_machine.general.current_state},
            .master_operational_state{&state_machine.nested.current_state},
            .slave_general_state{&spi.slave_general_state},
            .slave_operational_state{&spi.slave_operational_state},
        }};

    STLIBHandle stlib{};

    void initialize_state_machine();

    void update_connecting();
    void update_operational();
    void update_fault();

    void update_operational_idle();
    void update_operational_precharge();

   public:
    Board();
    void update();
};
};  // namespace BCU
