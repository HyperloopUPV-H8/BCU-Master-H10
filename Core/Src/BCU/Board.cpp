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

    Time::register_low_precision_alarm(1, [&]() {
        should_update_low_frequency = true;
        should_send_data_1khz = true;
    });

    Time::register_low_precision_alarm(16,
                                       [&]() { should_send_data_60hz = true; });

    Time::register_low_precision_alarm(50, [&]() { spi.sync_state(); });
};

void Board::update() {
    switch (state_machine.general.current_state) {
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

    if (should_send_data_1khz) {
        should_send_data_1khz = false;
        ethernet.send_commutation_details();
        ethernet.send_clarke_park_transform();
        ethernet.send_inverse_clarke_park_transform();
        ethernet.send_control_information();
        ethernet.send_current_sense();
        ethernet.send_encoder_sense();
    }
    if (should_send_data_60hz) {
        should_send_data_60hz = false;
        ethernet.send_motor_driver_sense();
        ethernet.send_temperature_sense();
        ethernet.send_state();
    }

    state_machine.general.check_transitions();
    protection_manager.update_high_frequency();
    stlib.update();
}

void Board::update_connecting() {}

void Board::update_operational() {
    switch (state_machine.nested.current_state) {
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
    using GeneralState = Shared::State::SharedStateMachine::GeneralState;
    using OperationalState = Shared::State::SharedStateMachine::NestedState;

    // General State Machine

    //     Transitions

    state_machine.general.add_transition(
        GeneralState::Connecting, GeneralState::Operational,
        [&]() { return ethernet.is_connected(); });

    state_machine.general.add_transition(
        GeneralState::Operational, GeneralState::Fault,
        [&]() { return spi.slave_general_state == GeneralState::Fault; });

    //     Enter Actions

    state_machine.general.add_enter_action(
        [&]() {
            leds.signal_connecting();
            ethernet.send_state();
        },
        GeneralState::Connecting);

    state_machine.general.add_enter_action(
        [&]() {
            leds.signal_operational();
            ethernet.send_state();
        },
        GeneralState::Operational);

    state_machine.general.add_enter_action(
        [&]() {
            motor_driver.disable_and_lock();
            leds.signal_fault();
            ethernet.send_state();
        },
        GeneralState::Fault);

    // Operational State Machine

    //     Enter Actions

    state_machine.nested.add_enter_action(
        [&]() {
            leds.signal_inverter_off();
            motor_driver.disable();
            ethernet.send_state();
        },
        OperationalState::Idle);

    state_machine.nested.add_enter_action([&]() { ethernet.send_state(); },
                                          OperationalState::Precharge);

    state_machine.nested.add_enter_action([&]() { ethernet.send_state(); },
                                          OperationalState::Ready);

    state_machine.nested.add_enter_action([&]() { ethernet.send_state(); },
                                          OperationalState::Testing);

    state_machine.nested.add_enter_action([&]() { ethernet.send_state(); },
                                          OperationalState::Boosting);

    //     Exit Actions

    state_machine.nested.add_exit_action(
        [&]() {
            leds.signal_inverter_on();
            motor_driver.enable();
        },
        OperationalState::Idle);
}

};  // namespace BCU
