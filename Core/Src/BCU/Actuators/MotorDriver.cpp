#include "BCU/Actuators/MotorDriver.hpp"

namespace BCU::Actuators {

void MotorDriver::turn_on() {
    for (size_t i{0}; i < 3; ++i) {
        buffer_enable[i].turn_off();
    }
}

void MotorDriver::turn_off() {
    for (size_t i{0}; i < 3; ++i) {
        buffer_enable[i].turn_on();
    }
}

bool MotorDriver::turn_off_and_lock() {
    return buffer_enable[0].lock_pin_state(PinState::ON) &&
           buffer_enable[1].lock_pin_state(PinState::ON) &&
           buffer_enable[2].lock_pin_state(PinState::ON);
}

};  // namespace BCU::Actuators
