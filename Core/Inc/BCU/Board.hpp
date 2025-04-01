#pragma once

#include "BCU/Communication/Ethernet.hpp"
#include "BCU/Communication/SPI.hpp"
#include "ST-LIB.hpp"

namespace BCU {

// Turn the STLIB into a class
struct STLIBHandle {
    STLIBHandle(string ip = "192.168.1.4", string subnet_mask = "255.255.0.0",
                string gateway = "192.168.1.1",
                UART::Peripheral& printf_peripheral = UART::uart2) {
        STLIB::start(ip, subnet_mask, gateway, printf_peripheral);
    }

    void update() { STLIB::update(); }
};

class Board {
   public:
    enum GeneralState : uint8_t { Connecting = 0, Operational = 1, Fault = 2 };
    enum OperationalState : uint8_t { Idle = 0, Ready = 1, Boosting = 2 };

   private:
    bool enable_booster{false};
    bool dc_poles_ready{false};

    StateMachine general_state_machine{GeneralState::Connecting};
    StateMachine operational_state_machine{OperationalState::Idle};

    Communication::SPI spi;

    STLIBHandle stlib;

    Communication::Ethernet ethernet;

    void initialize_state_machine();

    void update_connecting();
    void update_operational();
    void update_fault();

    void update_operational_idle();
    void update_operational_ready();
    void update_operational_boosting();

   public:
    Board();

    void update();
};

};  // namespace BCU