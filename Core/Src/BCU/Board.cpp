#include "BCU/Board.hpp"

namespace BCU {

using namespace Shared::State;

Board::Board()
    : motor_driver(Pinout::buffer_1_enable_pin, Pinout::buffer_2_enable_pin,
                   Pinout::buffer_3_enable_pin),
      spi(Pinout::spi_ready_slave_pin,
          &state_machine.general_state_machine.current_state,
          &state_machine.nested_state_machine.current_state),
      can(),
      stlib() {
    populate_state_machine();

    motor_driver.turn_off();

    spi.start();
    can.start();

    Time::register_low_precision_alarm(50, [&]() { spi.transmit_state(); });

    Time::register_low_precision_alarm(
        100, [&]() { spi.request_control_parameters(); });

    Time::register_low_precision_alarm(100, [&]() {
        can.transmit_state(state_machine.general_state_machine.current_state,
                           state_machine.nested_state_machine.current_state,
                           spi.slave_general_state, spi.slave_nested_state);
        can.transmit_control_parameters(spi.duty_cycle_u, spi.duty_cycle_v,
                                        spi.duty_cycle_w);
    });

    Time::register_low_precision_alarm(
        1, [&]() { protection_manager.update_low_frequency(); });
}

void Board::populate_state_machine() {
    // ***********
    // Transitions
    // ***********

    state_machine.general_state_machine.add_transition(
        SharedStateMachine::GeneralState::Connecting,
        SharedStateMachine::GeneralState::Operational, [&]() { return true; });

    state_machine.general_state_machine.add_transition(
        SharedStateMachine::GeneralState::Connecting,
        SharedStateMachine::GeneralState::Fault, [&]() {
            return spi.slave_general_state ==
                   SharedStateMachine::GeneralState::Fault;
        });

    state_machine.general_state_machine.add_transition(
        SharedStateMachine::GeneralState::Operational,
        SharedStateMachine::GeneralState::Fault, [&]() {
            return spi.slave_general_state ==
                   SharedStateMachine::GeneralState::Fault;
        });

    // ******************
    // Nested Transitions
    // ******************

    state_machine.nested_state_machine.add_transition(
        SharedStateMachine::NestedState::Idle,
        SharedStateMachine::NestedState::Testing, [&]() {
            return can.has_received_start_test_pwm ||
                   can.has_received_start_space_vector;
        });

    state_machine.nested_state_machine.add_transition(
        SharedStateMachine::NestedState::Testing,
        SharedStateMachine::NestedState::Idle,
        [&]() { return can.has_received_stop_control; });

    state_machine.nested_state_machine.add_enter_action(
        [&]() {
            motor_driver.turn_on();

            if (can.has_received_start_test_pwm) {
                spi.start_test_pwm(
                    ((float)can.requested_duty_cycle_u) / 100.0f,
                    ((float)can.requested_duty_cycle_v) / 100.0f,
                    ((float)can.requested_duty_cycle_w) / 100.0f);
            } else if (can.has_received_start_space_vector) {
                spi.start_space_vector(can.requested_modulation_index,
                                       can.requested_modulation_frequency_hz);
            }

            can.has_received_start_test_pwm = false;
            can.has_received_start_space_vector = false;
        },
        SharedStateMachine::NestedState::Testing);

    state_machine.nested_state_machine.add_enter_action(
        [&]() {
            motor_driver.turn_off();
            spi.stop_control();

            can.has_received_start_test_pwm = false;
            can.has_received_start_space_vector = false;
            can.has_received_stop_control = false;
        },
        SharedStateMachine::NestedState::Idle);

    state_machine.general_state_machine.add_enter_action(
        [&]() {
            motor_driver.turn_off();
            spi.stop_control();
        },
        SharedStateMachine::GeneralState::Fault);
}

void Board::update() {
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

    can.update();
    spi.update();
    stlib.update();
    protection_manager.update_high_frequency();
    state_machine.general_state_machine.check_transitions();
}

void Board::update_connecting() {}
void Board::update_operational() {
    if (can.has_received_configure_commutation_parameters) {
        spi.configure_commutation_parameters(
            (uint32_t)can.requested_commutation_frequency_hz,
            (uint32_t)can.requested_dead_time_ns);

        can.has_received_configure_commutation_parameters = false;
    }

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
        motor_driver.turn_off();
        spi.stop_control();

        can.has_received_start_test_pwm = false;
        can.has_received_stop_control = false;
    }
}

void Board::update_operational_ready() {}
void Board::update_operational_boosting() {}

void Board::update_operational_testing() {
    if (can.has_received_start_test_pwm) {
        spi.start_test_pwm(((float)can.requested_duty_cycle_u) / 100.0f,
                           ((float)can.requested_duty_cycle_v) / 100.0f,
                           ((float)can.requested_duty_cycle_w) / 100.0f);

        can.has_received_start_test_pwm = false;
    } else if (can.has_received_start_space_vector) {
        spi.start_space_vector(can.requested_modulation_index,
                               can.requested_modulation_frequency_hz);

        can.has_received_start_space_vector = false;
    }
}

};  // namespace BCU