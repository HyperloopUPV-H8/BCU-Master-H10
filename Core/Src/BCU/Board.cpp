#include "BCU/Board.hpp"

namespace BCU {

Board::Board()
    : spi(&general_state_machine.current_state,
          &operational_state_machine.current_state),
      stlib(BCU::Communication::Ethernet::local_ip.string_address),
      ethernet(&general_state_machine.current_state,
               &operational_state_machine.current_state,
               &spi.slave_general_state, &spi.slave_nested_state) {
    initialize_state_machine();

    spi.start();

    Time::register_low_precision_alarm(100, [&]() {
        spi.transmit_state();
        ethernet.send_board_state();
    });
}

void Board::initialize_state_machine() {
    general_state_machine.add_state(GeneralState::Operational);
    general_state_machine.add_state(GeneralState::Fault);

    operational_state_machine.add_state(OperationalState::Ready);
    operational_state_machine.add_state(OperationalState::Boosting);

    general_state_machine.add_state_machine(operational_state_machine,
                                            GeneralState::Operational);

    general_state_machine.add_transition(
        GeneralState::Connecting, GeneralState::Operational,
        [&]() { return ethernet.is_connected(); });

    general_state_machine.add_transition(
        GeneralState::Connecting, GeneralState::Fault,
        [&]() { return spi.slave_general_state == GeneralState::Fault; });

    general_state_machine.add_transition(
        GeneralState::Operational, GeneralState::Fault,
        [&]() { return spi.slave_general_state == GeneralState::Fault; });

    operational_state_machine.add_transition(OperationalState::Idle,
                                             OperationalState::Ready,
                                             [&]() { return enable_booster; });

    operational_state_machine.add_transition(
        OperationalState::Ready, OperationalState::Boosting, [&]() {
            return ethernet.dc_poles_state ==
                       Communication::Ethernet::DCPoleState::Ready &&
                   spi.slave_nested_state == OperationalState::Boosting;
        });

    operational_state_machine.add_transition(OperationalState::Ready,
                                             OperationalState::Idle,
                                             [&]() { return !enable_booster; });

    operational_state_machine.add_transition(OperationalState::Boosting,
                                             OperationalState::Idle,
                                             [&]() { return !enable_booster; });

    ProtectionManager::link_state_machine(general_state_machine,
                                          GeneralState::Fault);
}

void Board::update() {
    general_state_machine.check_transitions();
    switch (general_state_machine.current_state) {
        case GeneralState::Connecting:
            update_connecting();
            break;
        case GeneralState::Operational:
            update_operational();
            break;
        case GeneralState::Fault:
            update_fault();
            break;
    }

    spi.update();
    stlib.update();
}

void Board::update_connecting() {}
void Board::update_operational() {
    if (ethernet.has_received_disable_booster) {
        enable_booster = false;

        ethernet.has_received_enable_booster = false;
        ethernet.has_received_disable_booster = false;
    } else if (ethernet.has_received_enable_booster) {
        enable_booster = true;

        ethernet.has_received_enable_booster = false;
        ethernet.has_received_disable_booster = false;
    }

    switch (operational_state_machine.current_state) {
        case OperationalState::Idle:
            update_operational_idle();
            break;
        case OperationalState::Ready:
            update_operational_ready();
            break;
        case OperationalState::Boosting:
            update_operational_boosting();
            break;
    }
}
void Board::update_fault() {}

void Board::update_operational_idle() {}
void Board::update_operational_ready() {}
void Board::update_operational_boosting() {}

};  // namespace BCU