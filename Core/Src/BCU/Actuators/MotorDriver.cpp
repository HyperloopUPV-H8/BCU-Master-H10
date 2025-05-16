#include "BCU/Actuators/MotorDriver.hpp"

namespace BCU::Actuators {

void MotorDriver::enable() {
    buffer_enable[0].turn_off();
    buffer_enable[1].turn_off();
    buffer_enable[2].turn_off();
}

void MotorDriver::disable() {
    buffer_enable[0].turn_on();
    buffer_enable[1].turn_on();
    buffer_enable[2].turn_on();
}

bool MotorDriver::disable_and_lock() {
    return buffer_enable[0].lock_pin_state(PinState::ON) &&
           buffer_enable[1].lock_pin_state(PinState::ON) &&
           buffer_enable[2].lock_pin_state(PinState::ON);
}

};  // namespace BCU::Actuators