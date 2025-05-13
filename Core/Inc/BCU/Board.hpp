#pragma once

#include "BCU/Actuators/LEDs.hpp"
#include "ST-LIB.hpp"

namespace BCU {

class ProtectionManagerHandle {
   public:
    ProtectionManagerHandle(StateMachine& state_machine,
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
                UART::Peripheral& printf_peripheral = UART::uart2);
    void update();
};

class Board {
    enum GeneralState : uint8_t {
        Connecting = 0,
        Operational = 1,
        Fault = 2,
    };

    enum OperationalState : uint8_t {
        Idle = 0,
        Precharge = 1,
    };

    StateMachine general_state_machine{GeneralState::Connecting};
    StateMachine operational_state_machine{OperationalState::Idle};

    volatile bool should_update_low_frequency{false};
    ProtectionManagerHandle protection_manager{general_state_machine,
                                               GeneralState::Fault};

    Actuators::LEDs leds;

    STLIBHandle stlib{};

    void initialize_state_machine();

    void update_connecting();
    void update_operational();
    void update_fault();

    void update_operational_idle();
    void update_operational_precharge();

   public:
    Board();
    void update();
};

};  // namespace BCU