#pragma once

#include "BCU/Pins.hpp"
#include "ST-LIB.hpp"

namespace BCU::Actuators {
using namespace BCU;
class Contactors {
   private:
    DigitalOutput ContactorHigh;
    DigitalOutput ContactorPrecharge;
    DigitalOutput ContactorActiveDischarge;
#if USING_CONTACTOR_LOW
    DigitalOutput ContactorLow;
#endif

    constexpr static uint8_t UndefinedId{UINT8_MAX};
    uint8_t PrechargeCheckId{UndefinedId};
    uint8_t ActiveDischargeCheckId{UndefinedId};

    void close_main_circuit();
    void close_precharge_circuit();
    void close_active_discharge_circuit();

    void wait_for_precharge();
    void cancel_precharge();

    void wait_for_active_discharge();
    void cancel_active_discharge();

   public:
    enum class PrechargeStatus {
        NotStarted,
        Ongoing,
        Done,
        Failed,
    };

    enum class ActiveDischargeStatus {
        NotStarted,
        Ongoing,
        Done,
        Failed,
    };

    static PrechargeStatus LastPrechargeStatus;
    static ActiveDischargeStatus LastActiveDischargeStatus;

    Contactors()
        : ContactorHigh(Pins::ContactorHighPin),
          ContactorPrecharge(Pins::ContactorPrechargePin),
#if USING_CONTACTOR_LOW
          ContactorLow(Pins::ContactorLowPin),
#endif
          ContactorActiveDischarge(Pins::ContactorActiveDischargePin) {
    }

    // Opens all the contactors
    void open_all();

    // Close the main circuit (High and Low), but precharges the circuit before
    // doing so (Precharge and Low)
    void close_with_precharge();

    // Close the active discharge circuit (ActiveDischarge and Low)
    void close_active_discharge();
};

}  // namespace BCU::Actuators