#pragma once

#include "BCU/Actuators/Contactors.hpp"
#include "BCU/Actuators/LEDs.hpp"
#include "BCU/Communication/Ethernet.hpp"
#include "BCU/Communication/FDCAN.hpp"
#include "ST-LIB.hpp"

namespace BCU {
class Board {
   private:
    inline static bool started{false};

    void initialize_state_machine();

    static bool is_connected();
    static bool has_started() { return started; }
    static bool has_failed_precharge();
    static bool has_failed_active_discharge();

    void update_connecting();
    void update_operational();
    void update_fault();

    inline static bool send_supercaps_data{false};
    inline static bool update_fdcan{false};

   public:
    static Board* instance;
    enum GeneralStates : uint8_t {
        Idle,
        Connecting,
        Operational,
        Fault,
    };

    Actuators::Contactors contactors{};
    Actuators::LEDs leds{};

    StateMachine GeneralStateMachine;

    Board() {
        // This constructor is called twice, but since on the first call the
        // DigitalOutputService map has not yet been initialized, the old
        // entries are destroyed and we need to do this here actually

        instance = this;
        Communication::FDCAN::initialize();
        //STLIB::start(IPV4("192.168.0.15"), IPV4("255.255.255.0"),
                     //IPV4("192.168.0.1"), UART::uart2);
        STLIB::start(IPV4("127.0.0.1"), IPV4("255.255.255.0"),
                     IPV4("127.0.0.1"), UART::uart2);
        Communication::Ethernet::initialize();
        leds.signal_can_started();
        initialize_state_machine();

        start();
        started = true;
    }

    void start();
    void update();
};
}  // namespace BCU