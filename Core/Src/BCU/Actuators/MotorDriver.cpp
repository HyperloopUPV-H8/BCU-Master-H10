#include "BCU/Actuators/MotorDriver.hpp"

namespace BCU::Actuators {

MotorDriver::MotorDriver(Pin& buffer_1_enable_pin, Pin& buffer_2_enable_pin,
                         Pin& buffer_3_enable_pin)
    : buffer_enable{DigitalOutput{buffer_1_enable_pin},
                    DigitalOutput{buffer_2_enable_pin},
                    DigitalOutput{buffer_3_enable_pin}} {}

void MotorDriver::turn_on() {
    for (auto& buffer : buffer_enable) {
        buffer.turn_off();  // Enable @ logic level low
    }
}

void MotorDriver::turn_off() {
    for (auto& buffer : buffer_enable) {
        buffer.turn_on();  // Disable @ logic level high
    }
}

}  // namespace BCU::Actuators