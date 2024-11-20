#pragma once

namespace BCU::Sensors {
class SuperCapacitors {
   private:
   public:
    static float module_voltage[3];

    static float get_total_voltage() {
        return module_voltage[0] + module_voltage[1] + module_voltage[2];
    }
};
}  // namespace BCU::Sensors