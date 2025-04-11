#include "BCU/Board.hpp"

namespace BCU {

using namespace Shared::State;

Board::Board()
    : spi(Pinout::spi_ready_slave_pin,
          &state_machine.general_state_machine.current_state,
          &state_machine.nested_state_machine.current_state),
      leds(Pinout::led_operational_pin, Pinout::led_fault_pin,
           Pinout::led_can_pin, Pinout::led_flash_pin, Pinout::led_sleep_pin),
      motor_driver(Pinout::buffer_1_enable_pin, Pinout::buffer_2_enable_pin,
                   Pinout::buffer_3_enable_pin),
      can(),
      stlib() {
    populate_state_machine();

    leds.signal_connecting();
    motor_driver.disable_buffer();

    spi.start();
    can.start();

    Time::register_low_precision_alarm(50, [&]() {
        spi.transmit_state();
        spi.read_control_parameters();
        spi.read_motor_drivers();
        spi.read_position_encoder();
    });

    Time::register_low_precision_alarm(
        1, [&]() { protection_manager.update_low_frequency(); });
}

void Board::populate_state_machine() {
    state_machine.general_state_machine.add_transition(
        SharedStateMachine::GeneralState::Connecting,
        SharedStateMachine::GeneralState::Operational, [&]() { return true; });

    state_machine.general_state_machine.add_transition(
        SharedStateMachine::GeneralState::Connecting,
        SharedStateMachine::GeneralState::Fault, [&]() {
            return spi.slave_general_state ==
                       SharedStateMachine::GeneralState::Fault ||
                   can.has_received_fault;
        });

    state_machine.general_state_machine.add_transition(
        SharedStateMachine::GeneralState::Operational,
        SharedStateMachine::GeneralState::Fault, [&]() {
            return spi.slave_general_state ==
                       SharedStateMachine::GeneralState::Fault ||
                   can.has_received_fault;
        });

    state_machine.nested_state_machine.add_transition(
        SharedStateMachine::NestedState::Idle,
        SharedStateMachine::NestedState::Testing, [&]() {
            return can.has_received_start_test_pwm ||
                   can.has_received_start_emulated_movement;
        });

    state_machine.nested_state_machine.add_transition(
        SharedStateMachine::NestedState::Testing,
        SharedStateMachine::NestedState::Idle,
        [&]() { return can.has_received_stop_control; });

    state_machine.nested_state_machine.add_transition(
        SharedStateMachine::NestedState::Ready,
        SharedStateMachine::NestedState::Idle,
        [&]() { return can.has_received_stop_control; });

    state_machine.nested_state_machine.add_enter_action(
        [&]() {
            motor_driver.enable_buffer();
            if (can.has_received_start_test_pwm) {
                spi.start_test_pwm(can.requested_duty_cycle_u,
                                   can.requested_duty_cycle_v,
                                   can.requested_duty_cycle_w);
            } else if (can.has_received_start_emulated_movement) {
                spi.start_emulated_movement(can.requested_d_current_reference,
                                            can.requested_q_current_reference,
                                            can.requested_angular_velocity);
            }

            can.has_received_start_test_pwm = false;
            can.has_received_start_emulated_movement = false;
        },
        SharedStateMachine::NestedState::Testing);

    state_machine.nested_state_machine.add_enter_action(
        [&]() {
            motor_driver.disable_buffer();
            spi.stop_control();

            can.has_received_start_test_pwm = false;
            can.has_received_start_emulated_movement = false;
            can.has_received_stop_control = false;
        },
        SharedStateMachine::NestedState::Idle);

    state_machine.general_state_machine.add_enter_action(
        [&]() {
            motor_driver.disable_buffer();
            spi.stop_control();
        },
        SharedStateMachine::GeneralState::Fault);

    state_machine.general_state_machine.add_enter_action(
        [&]() { leds.signal_operational(); },
        SharedStateMachine::GeneralState::Operational);

    state_machine.general_state_machine.add_enter_action(
        [&]() { leds.signal_fault(); },
        SharedStateMachine::GeneralState::Fault);

    state_machine.general_state_machine.add_enter_action(
        [&]() { spi.transmit_state(); },
        SharedStateMachine::GeneralState::Connecting);

    state_machine.general_state_machine.add_enter_action(
        [&]() { spi.transmit_state(); },
        SharedStateMachine::GeneralState::Operational);

    state_machine.general_state_machine.add_enter_action(
        [&]() { spi.transmit_state(); },
        SharedStateMachine::GeneralState::Fault);

    state_machine.nested_state_machine.add_enter_action(
        [&]() { spi.transmit_state(); }, SharedStateMachine::NestedState::Idle);

    state_machine.nested_state_machine.add_enter_action(
        [&]() { spi.transmit_state(); },
        SharedStateMachine::NestedState::Ready);

    state_machine.nested_state_machine.add_enter_action(
        [&]() { spi.transmit_state(); },
        SharedStateMachine::NestedState::Boosting);

    state_machine.nested_state_machine.add_enter_action(
        [&]() { spi.transmit_state(); },
        SharedStateMachine::NestedState::Testing);
}

void Board::update() {
    state_machine.general_state_machine.check_transitions();

    switch (state_machine.general_state_machine.current_state) {
        case SharedStateMachine::GeneralState::Connecting:
            update_connecting();
            break;
        case SharedStateMachine::GeneralState::Operational:
            update_operational();
            break;
        case SharedStateMachine::GeneralState::Fault:
            update_fault();
            break;
    }

    protection_manager.update_high_frequency();
    can.update();
    spi.update();
    stlib.update();
}

void Board::update_connecting() {}
void Board::update_operational() {
    switch (state_machine.nested_state_machine.current_state) {
        case SharedStateMachine::NestedState::Idle:
            update_operational_idle();
            break;
        case SharedStateMachine::NestedState::Ready:
            update_operational_ready();
            break;
        case SharedStateMachine::NestedState::Boosting:
            update_operational_boosting();
            break;
        case SharedStateMachine::NestedState::Testing:
            update_operational_testing();
            break;
    }
}
void Board::update_fault() {}

void Board::update_operational_idle() {
    if (can.has_received_stop_control) {
        motor_driver.disable_buffer();
        spi.stop_control();

        can.has_received_start_test_pwm = false;
        can.has_received_start_emulated_movement = false;
        can.has_received_stop_control = false;
    }
}

void Board::update_operational_ready() {}
void Board::update_operational_boosting() {}

void Board::update_operational_testing() {
    if (can.has_received_start_test_pwm) {
        spi.start_test_pwm(can.requested_duty_cycle_u,
                           can.requested_duty_cycle_v,
                           can.requested_duty_cycle_w);

        can.has_received_start_test_pwm = false;
        can.has_received_start_emulated_movement = false;
    } else if (can.has_received_start_emulated_movement) {
        spi.start_emulated_movement(can.requested_d_current_reference,
                                    can.requested_q_current_reference,
                                    can.requested_angular_velocity);

        can.has_received_start_test_pwm = false;
        can.has_received_start_emulated_movement = false;
    }
}
};  // namespace BCU