#pragma once

#include "BCU/Pins.hpp"
#include "ST-LIB.hpp"

namespace BCU::Actuators {
using namespace BCU;
class Contactors {
   private:
    class NormallyOpenContactor {
        DigitalOutput output;

       public:
        NormallyOpenContactor(Pin& pin) : output(pin) {}

        void close() { output.turn_on(); }
        void open() { output.turn_off(); }
        void to_normal_state() { open(); }
    };

    class NormallyClosedContactor {
        DigitalOutput output;

       public:
        NormallyClosedContactor(Pin& pin) : output(pin) {}

        void close() { output.turn_off(); }
        void open() { output.turn_on(); }
        void to_normal_state() { close(); }
    };

    NormallyOpenContactor contactor_high_side;
    NormallyOpenContactor contactor_precharge;
#if USING_CONTACTOR_LOW_SIDE
    NormallyOpenContactor contactor_low_side;
#endif
#if USING_CONTACTOR_INVERSOR_DISCHARGE
    NormallyClosedContactor contactor_inversor_discharge;
#endif
#if USING_CONTACTOR_SUPERCAPS_CHARGE
    NormallyOpenContactor contactor_supercaps_charge;
#else
    NormallyClosedContactor contactor_supercaps_discharge;
#endif

    constexpr static uint8_t undefined_id{UINT8_MAX};
    constexpr static uint32_t precharge_check_period_ms{5000};
    uint8_t precharge_check_id{undefined_id};

    void to_normal_state();

    void to_precharge();

    void to_closed();

   public:
    Contactors();

    void open();

    void close();

    void charge();
};

}  // namespace BCU::Actuators