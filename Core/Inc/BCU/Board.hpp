#pragma once

#include "BCU/Actuators/LEDs.hpp"
#include "BCU/Actuators/MotorDriver.hpp"
#include "BCU/Communication/Ethernet.hpp"
#include "BCU/Communication/SPI.hpp"
#include "BCU/Sensors/Temperature.hpp"
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
    using GeneralState = Shared::State::SharedStateMachine::GeneralState;
    using OperationalState = Shared::State::SharedStateMachine::NestedState;

    constexpr static double MIN_SPEED_FOR_BOOSTER{0.1};

    volatile bool should_update_low_frequency{false};
    volatile bool should_send_data_1khz{false};
    volatile bool should_send_data_60hz{false};
    volatile bool should_read_temperature{false};

    bool dc_poles_requested{false};
    volatile bool dc_poles_ready{false};

    Shared::State::SharedStateMachine state_machine;

    ProtectionManagerHandle protection_manager{state_machine.general,
                                               GeneralState::Fault};

    Sensors::Temperature temperature_sense;

    Actuators::LEDs leds;
    Actuators::MotorDriver motor_driver;

    Communication::SPI spi{::SPI::spi3, &state_machine.general.current_state,
                           &state_machine.nested.current_state};

    STLIBHandle stlib;

    Communication::Ethernet ethernet{
        Communication::Ethernet::CommutationDetailsData{
            .duty_cycle_u{&spi.duty_cycle_u},
            .duty_cycle_v{&spi.duty_cycle_v},
            .duty_cycle_w{&spi.duty_cycle_w},
            .commutation_frequency_hz{&spi.commutation_frequency_hz},
            .dead_time_ns{&spi.dead_time_ns},
        },
        Communication::Ethernet::ClarkeParkTransformData{
            .measured_current_d{&spi.measured_current_d},
            .measured_current_q{&spi.measured_current_q},
            .measured_current_alpha{&spi.measured_current_alpha},
            .measured_current_beta{&spi.measured_current_beta},
            .measured_current_u{&spi.measured_current_u},
            .measured_current_v{&spi.measured_current_v},
            .measured_current_w{&spi.measured_current_w},
            .electrical_angle{&spi.electrical_angle},
        },
        Communication::Ethernet::InverseClarkeParkTransformData{
            .target_voltage_d{&spi.target_voltage_d},
            .target_voltage_q{&spi.target_voltage_q},
            .target_voltage_alpha{&spi.target_voltage_alpha},
            .target_voltage_beta{&spi.target_voltage_beta},
            .target_voltage_u{&spi.target_voltage_u},
            .target_voltage_v{&spi.target_voltage_v},
            .target_voltage_w{&spi.target_voltage_w},
            .dc_link_voltage{&spi.dc_link_voltage},
        },
        Communication::Ethernet::ControlInformationData{
            .reference_current_d{&spi.reference_current_d},
            .reference_current_q{&spi.reference_current_q},
            .reference_speed{&spi.reference_speed},
            .error_current_d{&spi.error_current_d},
            .error_current_q{&spi.error_current_q},
            .error_speed{&spi.error_speed},
        },
        Communication::Ethernet::CurrentSenseData{
            .measured_current_u_1{&spi.measured_current_u_1},
            .measured_current_v_1{&spi.measured_current_v_1},
            .measured_current_w_1{&spi.measured_current_w_1},
            .measured_current_u_2{&spi.measured_current_u_2},
            .measured_current_v_2{&spi.measured_current_v_2},
            .measured_current_w_2{&spi.measured_current_w_2},
            .measured_current_u_3{&spi.measured_current_u_3},
            .measured_current_v_3{&spi.measured_current_v_3},
            .measured_current_w_3{&spi.measured_current_w_3},
            .measured_current_u_4{&spi.measured_current_u_4},
            .measured_current_v_4{&spi.measured_current_v_4},
            .measured_current_w_4{&spi.measured_current_w_4},
        },
        Communication::Ethernet::EncoderSenseData{
            .speetec_1_position{&spi.speetec_1_position},
            .speetec_1_velocity{&spi.speetec_1_velocity},
            .speetec_1_acceleration{&spi.speetec_1_acceleration},
            .speetec_2_position{&spi.speetec_2_position},
            .speetec_2_velocity{&spi.speetec_2_velocity},
            .speetec_2_acceleration{&spi.speetec_2_acceleration},
            .speetec_3_position{&spi.speetec_3_position},
            .speetec_3_velocity{&spi.speetec_3_velocity},
            .speetec_3_acceleration{&spi.speetec_3_acceleration},
            .speetec_position{&spi.speetec_position},
            .speetec_velocity{&spi.speetec_velocity},
            .speetec_acceleration{&spi.speetec_acceleration},
        },
        Communication::Ethernet::MotorDriverSenseData{
            .driver_1_dc_link_voltage{&spi.driver_1_dc_link_voltage},
            .driver_1_ready{&spi.driver_1_ready},
            .driver_1_fault{&spi.driver_1_fault},
            .driver_2_dc_link_voltage{&spi.driver_2_dc_link_voltage},
            .driver_2_ready{&spi.driver_2_ready},
            .driver_2_fault{&spi.driver_2_fault},
            .driver_3_dc_link_voltage{&spi.driver_3_dc_link_voltage},
            .driver_3_ready{&spi.driver_3_ready},
            .driver_3_fault{&spi.driver_3_fault},
            .driver_4_dc_link_voltage{&spi.driver_4_dc_link_voltage},
            .driver_4_ready{&spi.driver_4_ready},
            .driver_4_fault{&spi.driver_4_fault},
        },
        Communication::Ethernet::TemperatureSenseData{
            .motor_1_temperature{temperature_sense.get_motor_temperature(0)},
            .motor_2_temperature{temperature_sense.get_motor_temperature(1)},
            .motor_3_temperature{temperature_sense.get_motor_temperature(2)},
            .motor_4_temperature{temperature_sense.get_motor_temperature(3)},
            .driver_1_temperature{temperature_sense.get_driver_temperature(0)},
            .driver_2_temperature{temperature_sense.get_driver_temperature(1)},
            .driver_3_temperature{temperature_sense.get_driver_temperature(2)},
            .driver_4_temperature{temperature_sense.get_driver_temperature(3)},
        },
        Communication::Ethernet::StateData{
            .master_general_state{&state_machine.general.current_state},
            .master_operational_state{&state_machine.nested.current_state},
            .slave_general_state{&spi.slave_general_state},
            .slave_operational_state{&spi.slave_operational_state},
        },
    };


    void initialize_state_machine();
    void initialize_protections();

    void update_connecting();
    void update_operational();
    void update_fault();

    void update_operational_idle();
    void update_operational_precharge();
    void update_operational_ready();
    void update_operational_boosting();
    void update_operational_testing();

   public:
    Board();

    void update();
};

};  // namespace BCU
