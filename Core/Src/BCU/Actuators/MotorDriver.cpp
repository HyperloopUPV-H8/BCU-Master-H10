#include "BCU/Actuators/MotorDriver.hpp"

namespace BCU::Actuators {

MotorDriver::MotorDriver(Pin& buffer_1_enable_pin, Pin& buffer_2_enable_pin,
                         Pin& buffer_3_enable_pin)
    : buffer_1_enable(buffer_1_enable_pin),
      buffer_2_enable(buffer_2_enable_pin),
      buffer_3_enable(buffer_3_enable_pin) {}

void MotorDriver::enable_buffer() {
    buffer_1_enable.turn_off();
    buffer_2_enable.turn_off();
    buffer_3_enable.turn_off();
}

void MotorDriver::disable_buffer() {
    buffer_1_enable.turn_on();
    buffer_2_enable.turn_on();
    buffer_3_enable.turn_on();
}

}  // namespace BCU::Actuators