#include "BCU/BCU.hpp"

void BCU::BCU::initialize_state_machine() {
    GeneralStateMachine = StateMachine(GeneralStates::Connecting);
    GeneralStateMachine.add_state(GeneralStates::Operational);
    GeneralStateMachine.add_state(GeneralStates::Fault);

    GeneralStateMachine.add_transition(
        GeneralStates::Connecting, GeneralStates::Operational, is_connected);
    GeneralStateMachine.add_transition(
        GeneralStates::Operational, GeneralStates::Fault, has_failed_precharge);
    GeneralStateMachine.add_transition(GeneralStates::Operational,
                                       GeneralStates::Fault,
                                       has_failed_active_discharge);

    GeneralStateMachine.add_enter_action([&]() { contactors.open_all(); },
                                         GeneralStates::Fault);
}

void BCU::BCU::update() {
    STLIB::update();
    GeneralStateMachine.check_transitions();
    Communication::FDCAN::update();
    switch (GeneralStateMachine.current_state) {
        case GeneralStates::Connecting:
            update_connecting();
            break;
        case GeneralStates::Operational:
            update_operational();
            break;
        case GeneralStates::Fault:
            update_fault();
            break;
    }
}

void BCU::BCU::update_connecting() {}
void BCU::BCU::update_operational() {
    if (Communication::Ethernet::open_contactors_received) {
        contactors.open_all();
        Communication::Ethernet::reset_contactor_orders_received();
    } else if (Communication::Ethernet::close_main_circuit_received) {
        contactors.close_with_precharge();
        Communication::Ethernet::reset_contactor_orders_received();
    } else if (Communication::Ethernet::close_active_discharge_received) {
        contactors.close_active_discharge();
        Communication::Ethernet::reset_contactor_orders_received();
    }

    if (Communication::Ethernet::request_data_received) {
        Communication::FDCAN::request_data_to(
            1, CMS::DataRequestFlags::SendModuleState, 0);
        Communication::Ethernet::request_data_received = false;
    }
}
void BCU::BCU::update_fault() {}

bool BCU::BCU::has_failed_precharge() {
    return Actuators::Contactors::LastPrechargeStatus ==
           Actuators::Contactors::PrechargeStatus::Failed;
}

bool BCU::BCU::has_failed_active_discharge() {
    return Actuators::Contactors::LastActiveDischargeStatus ==
           Actuators::Contactors::ActiveDischargeStatus::Failed;
}

bool BCU::BCU::is_connected() {
    return Communication::Ethernet::is_connected();
}