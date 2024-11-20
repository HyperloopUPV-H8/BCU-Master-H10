#include "BCU/Actuators/Contactors.hpp"

#include "BCU/Config.hpp"
#include "BCU/Sensors/BusVoltage.hpp"
#include "BCU/Sensors/SuperCapacitors.hpp"

using namespace BCU::Actuators;

Contactors::PrechargeStatus Contactors::LastPrechargeStatus{
    PrechargeStatus::NotStarted};
Contactors::ActiveDischargeStatus Contactors::LastActiveDischargeStatus{
    ActiveDischargeStatus::NotStarted};

void Contactors::open_all() {
    cancel_precharge();
    cancel_active_discharge();

    ContactorHigh.turn_off();
    ContactorPrecharge.turn_off();
    ContactorActiveDischarge.turn_off();
#if USING_CONTACTOR_LOW
    ContactorLow.turn_off();
#endif
}

void Contactors::close_main_circuit() {
    cancel_precharge();
    cancel_active_discharge();

    ContactorHigh.turn_on();
    ContactorPrecharge.turn_off();
    ContactorActiveDischarge.turn_off();
#if USING_CONTACTOR_LOW
    ContactorLow.turn_on();
#endif
}

void Contactors::close_precharge_circuit() {
    cancel_active_discharge();

    ContactorHigh.turn_off();
    ContactorPrecharge.turn_on();
    ContactorActiveDischarge.turn_off();
#if USING_CONTACTOR_LOW
    ContactorLow.turn_on();
#endif
}

void Contactors::close_active_discharge_circuit() {
    cancel_precharge();

    ContactorHigh.turn_off();
    ContactorPrecharge.turn_off();
    ContactorActiveDischarge.turn_on();
#if USING_CONTACTOR_LOW
    ContactorLow.turn_off();
#endif
}

void Contactors::close_with_precharge() {
    LastPrechargeStatus = PrechargeStatus::Ongoing;
    close_precharge_circuit();
    wait_for_precharge();
}

#if PRECHARGE_CHECK == PRECHARGE_CHECK_RISING_VOLTAGE
constexpr float min_voltage_rise_between_checks{5.0f};
constexpr float required_output_to_bus_voltage_ratio{0.9};

void Contactors::wait_for_precharge() {
    if (PrechargeCheckId != UndefinedId) return;

    static float last_voltage;
    last_voltage = 0.0f;

    PrechargeCheckId = Time::register_low_precision_alarm(500, [&]() {
        const float required_bus_voltage{
            BCU::Sensors::SuperCapacitors::get_total_voltage() *
            required_output_to_bus_voltage_ratio};

        float current_voltage = BCU::Sensors::BusVoltage::get_average();
        if (current_voltage - last_voltage < min_voltage_rise_between_checks) {
            LastPrechargeStatus = PrechargeStatus::Failed;
            open_all();
        } else if (current_voltage >= required_bus_voltage) {
            LastPrechargeStatus = PrechargeStatus::Done;
            close_main_circuit();
        }
        last_voltage = current_voltage;
    });
}

void Contactors::cancel_precharge() {
    if (PrechargeCheckId == UndefinedId) return;

    Time::unregister_low_precision_alarm(PrechargeCheckId);
    PrechargeCheckId = UndefinedId;
}

#else
#if PRECHARGE_CHECK == PRECHARGE_CHECK_AFTER_TIMEOUT
constexpr float required_output_to_bus_voltage_ratio{0.8};
constexpr int precharge_timeout_ms{5000};

void Contactors::wait_for_precharge() {
    if (PrechargeCheckId != UndefinedId) return;

    PrechargeCheckId = Time::set_timeout(precharge_timeout_ms, [&]() {
        const float required_bus_voltage{
            BCU::Sensors::SuperCapacitors::get_total_voltage() *
            required_output_to_bus_voltage_ratio};
        const float current_voltage = BCU::Sensors::BusVoltage::get_average();

        if (current_voltage >= required_bus_voltage) {
            LastPrechargeStatus = PrechargeStatus::Done;
            close_main_circuit();
        } else {
            LastPrechargeStatus = PrechargeStatus::Failed;
            open_all();
        }
    });
}
void Contactors::cancel_precharge() {
    if (PrechargeCheckId == UndefinedId) return;

    Time::cancel_timeout(PrechargeCheckId);
    PrechargeCheckId = UndefinedId;
}
#else
constexpr int precharge_timeout_ms{5000};

void Contactors::wait_for_precharge() {
    if (PrechargeCheckId != UndefinedId) return;

    PrechargeCheckId = Time::set_timeout(precharge_timeout_ms, [&]() {
        LastPrechargeStatus = PrechargeStatus::Done;
        close_main_circuit();
    });
}
void Contactors::cancel_precharge() {
    if (PrechargeCheckId == UndefinedId) return;

    Time::cancel_timeout(PrechargeCheckId);
    PrechargeCheckId = UndefinedId;
}
#endif
#endif

void Contactors::close_active_discharge() {
    LastActiveDischargeStatus = ActiveDischargeStatus::Ongoing;
    close_active_discharge_circuit();
    wait_for_active_discharge();
}

#if ACTIVE_DISCHARGE_CHECK
constexpr float min_voltage_drop_between_checks{5.0f};
constexpr float safe_bus_voltage{48};

void Contactors::wait_for_active_discharge() {
    if (ActiveDischargeCheckId != UndefinedId) return;

    static float last_voltage;
    last_voltage = 0.0f;

    ActiveDischargeCheckId = Time::register_low_precision_alarm(500, [&]() {
        float current_voltage = BCU::Sensors::BusVoltage::get_average();
        if (last_voltage - current_voltage < min_voltage_drop_between_checks) {
            LastActiveDischargeStatus = ActiveDischargeStatus::Failed;
            open_all();
        } else if (current_voltage <= safe_bus_voltage) {
            LastActiveDischargeStatus = ActiveDischargeStatus::Done;
            open_all();
        }
        last_voltage = current_voltage;
    });
}
void Contactors::cancel_active_discharge() {
    if (PrechargeCheckId == UndefinedId) return;

    Time::unregister_low_precision_alarm(PrechargeCheckId);
    PrechargeCheckId = UndefinedId;
}
#else
void Contactors::wait_for_active_discharge() { return; }
void Contactors::cancel_active_discharge() {
    LastActiveDischargeStatus = ActiveDischargeStatus::Done;
}
#endif