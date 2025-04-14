#pragma once

#include "ST-LIB.hpp"

namespace BCU::Actuators {

class MotorDriver {
    std::array<DigitalOutput, 3> buffer_enable;

   public:
    MotorDriver(Pin& buffer_1_enable_pin, Pin& buffer_2_enable_pin,
                Pin& buffer_3_enable_pin);

    void turn_on();
    void turn_off();
};

};  // namespace BCU::Actuators