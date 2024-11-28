#pragma once

#include "BCU/Communication/FDCAN.hpp"

namespace BCU::Sensors {
class SuperCapacitors {
   private:
   public:
    static float get_total_voltage() {
        using BCU::Communication::ModuleCAN;

        return ModuleCAN::strings[1].modules[1].module_voltage +
               ModuleCAN::strings[1].modules[2].module_voltage +
               ModuleCAN::strings[1].modules[3].module_voltage;
    }
};
}  // namespace BCU::Sensors