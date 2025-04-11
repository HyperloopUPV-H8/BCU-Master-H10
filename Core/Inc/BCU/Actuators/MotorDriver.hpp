#pragma once

#include "ST-LIB.hpp"

namespace BCU::Actuators {

class MotorDriver {
    DigitalOutput buffer_1_enable;
    DigitalOutput buffer_2_enable;
    DigitalOutput buffer_3_enable;

   public:
    MotorDriver(Pin& buffer_1_enable_pin, Pin& buffer_2_enable_pin,
                Pin& buffer_3_enable_pin);

    void enable_buffer();
    void disable_buffer();
};

};  // namespace BCU::Actuators