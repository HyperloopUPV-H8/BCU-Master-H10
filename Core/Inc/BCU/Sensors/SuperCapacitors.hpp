#pragma once

#include "CMS-LIB.hpp"

namespace BCU::Sensors {
class SuperCapacitors {
   public:
    static const std::array<float &, 3> module_voltages;

    inline static float calculate_total_voltage();
};
}  // namespace BCU::Sensors