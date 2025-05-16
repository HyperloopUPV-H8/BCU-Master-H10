#include "BCU/Board.hpp"

namespace BCU {

ProtectionManagerHandle::ProtectionManagerHandle(
    StateMachine& state_machine, StateMachine::state_id fault_state) {
    ProtectionManager::link_state_machine(state_machine, fault_state);
    ProtectionManager::add_standard_protections();
}

void ProtectionManagerHandle::start() { ProtectionManager::initialize(); }

void ProtectionManagerHandle::update_high_frequency() {
    ProtectionManager::check_high_frequency_protections();
}

void ProtectionManagerHandle::update_low_frequency() {
    ProtectionManager::check_protections();
}

STLIBHandle::STLIBHandle(string mac, string ip, string subnet_mask,
                         string gateway, UART::Peripheral& printf_peripheral) {
    STLIB::start(mac, ip, subnet_mask, gateway, printf_peripheral);
}

void STLIBHandle::update() { STLIB::update(); }

Board::Board() {
    motor_driver.disable();
    leds.signal_connecting();

    initialize_state_machine();

    Time::register_low_precision_alarm(
        1, [&]() { should_update_low_frequency = true; });
};

void Board::update() {
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

    if (should_update_low_frequency) {
        should_update_low_frequency = false;
        protection_manager.update_low_frequency();
    }

    general_state_machine.check_transitions();
    protection_manager.update_high_frequency();
    stlib.update();
}

void Board::update_connecting() {}

void Board::update_operational() {
    switch (operational_state_machine.current_state) {
        case OperationalState::Idle:
            update_operational_idle();
            break;
        case OperationalState::Precharge:
            update_operational_precharge();
            break;
    }
}

void Board::update_fault() {}

void Board::update_operational_idle() {}

void Board::update_operational_precharge() {}

void Board::initialize_state_machine() {
    // General State Machine

    general_state_machine.add_state(GeneralState::Operational);
    general_state_machine.add_state(GeneralState::Fault);

    //     Enter Actions

    general_state_machine.add_enter_action([&]() { leds.signal_connecting(); },
                                           GeneralState::Connecting);

    general_state_machine.add_enter_action([&]() { leds.signal_operational(); },
                                           GeneralState::Operational);

    general_state_machine.add_enter_action(
        [&]() {
            motor_driver.disable_and_lock();
            leds.signal_fault();
        },
        GeneralState::Fault);

    // Operational State Machine

    general_state_machine.add_state_machine(operational_state_machine,
                                            GeneralState::Operational);

    operational_state_machine.add_state(OperationalState::Precharge);

    //     Enter Actions

    operational_state_machine.add_enter_action(
        [&]() {
            leds.signal_inverter_off();
            motor_driver.disable();
        },
        OperationalState::Idle);

    //     Exit Actions

    operational_state_machine.add_exit_action(
        [&]() {
            leds.signal_inverter_on();
            motor_driver.enable();
        },
        OperationalState::Idle);
}

};  // namespace BCU