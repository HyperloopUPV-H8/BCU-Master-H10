#include "BCU/Board.hpp"

namespace BCU {

using namespace Shared::State;

Board::Board()
    : spi(Pinout::spi_ready_slave_pin,
          &state_machine.general_state_machine.current_state,
          &state_machine.nested_state_machine.current_state),
      leds(Pinout::led_operational_pin, Pinout::led_fault_pin,
           Pinout::led_can_pin, Pinout::led_flash_pin, Pinout::led_sleep_pin),
      stlib(BCU::Communication::Ethernet::local_ip.string_address),
      ethernet(&state_machine.general_state_machine.current_state,
               &state_machine.nested_state_machine.current_state,
               &spi.slave_general_state, &spi.slave_nested_state) {
    populate_state_machine();

    spi.start();

    Time::register_low_precision_alarm(100, [&]() {
        spi.transmit_state();
        ethernet.send_board_state();
    });

    Time::register_low_precision_alarm(
        1, [&]() { protection_manager.update_low_frequency(); });
}

void Board::populate_state_machine() {
    state_machine.general_state_machine.add_transition(
        SharedStateMachine::GeneralState::Connecting,
        SharedStateMachine::GeneralState::Operational,
        [&]() { return ethernet.is_connected(); });

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

    leds.signal_connecting();

    state_machine.general_state_machine.add_enter_action(
        [&]() { leds.signal_operational(); },
        SharedStateMachine::GeneralState::Operational);

    state_machine.general_state_machine.add_enter_action(
        [&]() { leds.signal_fault(); },
        SharedStateMachine::GeneralState::Fault);
}

void Board::update() {
    protection_manager.update_high_frequency();
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
    }
}
void Board::update_fault() {}

void Board::update_operational_idle() {}
void Board::update_operational_ready() {}
void Board::update_operational_boosting() {}

};  // namespace BCU