#include "BCU/Actuators/Contactors.hpp"

#include "BCU/Communication/FDCAN.hpp"
#include "BCU/Config.hpp"
#include "BCU/Sensors/BusVoltage.hpp"

using namespace BCU::Actuators;

Contactors::Contactors()
    : contactor_high_side(BCU::Pins::contactor_high_pin),
      contactor_precharge(BCU::Pins::contactor_precharge_pin),
#if USING_CONTACTOR_LOW_SIDE
      contactor_low_side(BCU::Pins::contactor_low_pin),
#endif
#if USING_CONTACTOR_INVERSOR_DISCHARGE
      contactor_inversor_discharge(BCU::Pins::contactor_inversor_discharge_pin),
#endif
#if USING_CONTACTOR_SUPERCAPS_CHARGE
      contactor_supercaps_charge(BCU::Pins::contactor_supercaps_charge_pin),
#endif
      contactor_supercaps_discharge(
          BCU::Pins::contactor_supercaps_discharge_pin) {
}

void Contactors::open() { 
    to_normal_state(); }

void Contactors::close() {
    to_precharge();
    // TODO: better checks
    Time::set_timeout(precharge_check_period_ms, [&]() { to_closed(); });
}

void Contactors::to_normal_state() {
    contactor_high_side.to_normal_state();
    contactor_precharge.to_normal_state();
#if USING_CONTACTOR_LOW_SIDE
    contactor_low_side.to_normal_state();
#endif
#if USING_CONTACTOR_SUPERCAPS_CHARGE
    contactor_supercaps_charge.to_normal_state();
#endif
#if USING_CONTACTOR_INVERSOR_DISCHARGE
    contactor_inversor_discharge.to_normal_state();
#endif
    contactor_supercaps_discharge.to_normal_state();
}

void Contactors::to_precharge() {
    contactor_high_side.open();
    contactor_precharge.close();
#if USING_CONTACTOR_LOW_SIDE
    contactor_low_side.close();
#endif
#if USING_CONTACTOR_SUPERCAPS_CHARGE
    contactor_supercaps_charge.open();
#endif
#if USING_CONTACTOR_INVERSOR_DISCHARGE
    contactor_inversor_discharge.open();
#endif
    contactor_supercaps_discharge.open();
}

void Contactors::to_closed() {
    contactor_high_side.close();
    contactor_precharge.open();
#if USING_CONTACTOR_LOW_SIDE
    contactor_low_side.close();
#endif
#if USING_CONTACTOR_SUPERCAPS_CHARGE
    contactor_supercaps_charge.open();
#endif
#if USING_CONTACTOR_INVERSOR_DISCHARGE
    contactor_inversor_discharge.open();
#endif
    contactor_supercaps_discharge.open();
}