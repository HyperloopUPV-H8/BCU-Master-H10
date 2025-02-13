#include "BCU/BCU.hpp"

void BCU::Board::initialize_state_machine() {
    GeneralStateMachine = StateMachine(GeneralStates::Idle);
    GeneralStateMachine.add_state(GeneralStates::Connecting);
    GeneralStateMachine.add_state(GeneralStates::Operational);
    GeneralStateMachine.add_state(GeneralStates::Fault);

    ProtectionManager::link_state_machine(GeneralStateMachine,
                                          GeneralStates::Fault);

    GeneralStateMachine.add_transition(GeneralStates::Idle,
                                       GeneralStates::Connecting, has_started);

    GeneralStateMachine.add_transition(
        GeneralStates::Connecting, GeneralStates::Operational, is_connected);

    GeneralStateMachine.add_transition(
        GeneralStates::Operational, GeneralStates::Fault, has_failed_precharge);

    GeneralStateMachine.add_enter_action([&]() { leds.signal_connecting(); },
                                         GeneralStates::Connecting);

    GeneralStateMachine.add_enter_action([&]() { leds.signal_operational(); },
                                         GeneralStates::Operational);

    GeneralStateMachine.add_enter_action([&]() { 
        contactors.open(); },
                                         GeneralStates::Fault);

    Time::register_low_precision_alarm(100,
                                       [&]() { send_supercaps_data = true; });
}

void BCU::Board::start() { Communication::FDCAN::start(); }

void BCU::Board::update() {
    GeneralStateMachine.check_transitions();
    Communication::Ethernet::update();
    Communication::FDCAN::update();
    if (send_supercaps_data) {
        Communication::Ethernet::send_supercaps_data();
        send_supercaps_data = false;
        
    }
    STLIB::update();
}

bool BCU::Board::has_failed_precharge() { return false; }

bool BCU::Board::is_connected() {
    return Communication::Ethernet::is_connected();
}