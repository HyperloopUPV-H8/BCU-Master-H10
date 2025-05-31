#include "BCU/Sensors/Temperature.hpp"

namespace BCU::Sensors {

void Temperature::read_motor_temperature() {
#if USING_MOTOR_TEMPERATURE
    for (size_t i = 0; i < 4; i++) {
        motor_temperature_sensor[i].read();
    }
#endif
}

void Temperature::read_driver_temperature() {
#if USING_DRIVER_TEMPERATURE
    for (size_t i = 0; i < 4; i++) {
        driver_temperature_sensor[i].read();
    }
#endif
}

float *Temperature::get_motor_temperature(uint8_t index) {
    if (index >= 3) return nullptr;

    return motor_temperature_sensor[index].get_value_pointer();
}

float *Temperature::get_driver_temperature(uint8_t index) {
    if (index >= 3) return nullptr;

    return driver_temperature_sensor[index].get_value_pointer();
}

};  // namespace BCU::Sensors