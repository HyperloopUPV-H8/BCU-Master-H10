#pragma once

#include "BCU/Actuators/Contactors.hpp"
#include "BCU/Sensors/SuperCapacitors.hpp"
#include "BCU/Communication/Ethernet.hpp"
#include "BCU/Communication/FDCAN.hpp"
#include "ST-LIB.hpp"

namespace BCU {
class BCU {
   private:
    Actuators::Contactors contactors;

    void initialize_state_machine();

    static bool is_connected();
    static bool has_failed_precharge();
    static bool has_failed_active_discharge();

    void update_connecting();
    void update_operational();
    void update_fault();

   public:
    enum GeneralStates : uint8_t {
        Connecting,
        Operational,
        Fault,
    };

    StateMachine GeneralStateMachine;

    BCU() {
        contactors = Actuators::Contactors();
        Communication::FDCAN::initialize();
        STLIB::start(IPV4("192.168.0.15"), IPV4("255.255.255.0"),
                     IPV4("192.168.0.1"), UART::uart2);
        Communication::Ethernet::initialize();
        initialize_state_machine();
    }

    void update();
};
}  // namespace BCU