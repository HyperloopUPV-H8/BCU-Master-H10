#pragma once

namespace BCU::Sensors {
class BusVoltage {
   private:
   public:
    static float Voltages[4];

    static float get_average() {
        return (Voltages[0] + Voltages[1] + Voltages[2] + Voltages[3]) / 4;
    }
};
}  // namespace BCU::Sensors