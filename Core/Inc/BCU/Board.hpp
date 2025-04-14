#pragma once

#include "BCU/Actuators/MotorDriver.hpp"
#include "BCU/Communication/CAN.hpp"
#include "BCU/Communication/SPI.hpp"
#include "BCU/Pinout.hpp"
#include "ST-LIB.hpp"
#include "Shared/StateMachine.hpp"

namespace BCU {

struct ProtectionManagerHandle {
    ProtectionManagerHandle(StateMachine& state_machine) {
        ProtectionManager::initialize();
        ProtectionManager::add_standard_protections();
        ProtectionManager::link_state_machine(
            state_machine,
            Shared::State::SharedStateMachine::GeneralState::Fault);
    }

    void update_low_frequency() { ProtectionManager::check_protections(); }
    void update_high_frequency() {
        ProtectionManager::check_high_frequency_protections();
    }
};

struct STLIBHandle {
    STLIBHandle(string ip = "192.168.1.4", string subnet_mask = "255.255.0.0",
                string gateway = "192.168.1.1",
                UART::Peripheral& printf_peripheral = UART::uart2) {
        STLIB::start(ip, subnet_mask, gateway, printf_peripheral);
    }

    void update() { STLIB::update(); }
};

class Board {
    Shared::State::SharedStateMachine state_machine{};
    ProtectionManagerHandle protection_manager{
        state_machine.general_state_machine};

    Actuators::MotorDriver motor_driver;

    Communication::SPI spi;
    Communication::CAN can;

    STLIBHandle stlib;

    void populate_state_machine();

    void update_connecting();
    void update_operational();
    void update_fault();

    void update_operational_idle();
    void update_operational_ready();
    void update_operational_boosting();
    void update_operational_testing();

   public:
    Board();

    void update();
};

};  // namespace BCU