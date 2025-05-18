#pragma once

#include "BCU/Pinout.hpp"
#include "ST-LIB.hpp"

namespace BCU::Actuators {

class MotorDriver {
    DigitalOutput buffer_enable[3]{{Pinout::buffer_enable_1_pin},
                                   {Pinout::buffer_enable_2_pin},
                                   {Pinout::buffer_enable_3_pin}};

   public:
    MotorDriver() = default;

    void enable();
    void disable();

    bool disable_and_lock();
};

};  // namespace BCU::Actuators
