#pragma once

#include "BCU/Config.hpp"
#include "BCU/Pinout.hpp"
#include "ST-LIB.hpp"

namespace BCU::Sensors {

class Temperature {
    constexpr static size_t motor_temperature_filter_order{100};
    constexpr static float motor_temperature_offset{0.0f};
    constexpr static float motor_temperature_slope{1.0f};

    constexpr static size_t driver_temperature_filter_order{100};
    constexpr static float driver_temperature_offset{0.0f};
    constexpr static float driver_temperature_slope{1.0f};

    float motor_temperature[4]{0.0f};
    float driver_temperature[4]{0.0f};

    MovingAverage<motor_temperature_filter_order> motor_temperature_filter[4]{};
    MovingAverage<driver_temperature_filter_order>
        driver_temperature_filter[4]{};

    FilteredLinearSensor<float, motor_temperature_filter_order>
        motor_temperature_sensor[4]{
            {
                Pinout::booster_temp_1_pin,
                motor_temperature_offset,
                motor_temperature_slope,
                &motor_temperature[0],
                motor_temperature_filter[0],
            },
            {
                Pinout::booster_temp_2_pin,
                motor_temperature_offset,
                motor_temperature_slope,
                &motor_temperature[1],
                motor_temperature_filter[1],
            },
            {
                Pinout::booster_temp_3_pin,
                motor_temperature_offset,
                motor_temperature_slope,
                &motor_temperature[2],
                motor_temperature_filter[2],
            },
            {
                Pinout::booster_temp_4_pin,
                motor_temperature_offset,
                motor_temperature_slope,
                &motor_temperature[3],
                motor_temperature_filter[3],
            },
        };

    FilteredLinearSensor<float, driver_temperature_filter_order>
        driver_temperature_sensor[4]{
            {
                Pinout::inverter_temp_1_pin,
                driver_temperature_offset,
                driver_temperature_slope,
                &driver_temperature[0],
                driver_temperature_filter[0],
            },
            {
                Pinout::inverter_temp_2_pin,
                driver_temperature_offset,
                driver_temperature_slope,
                &driver_temperature[1],
                driver_temperature_filter[1],
            },
            {
                Pinout::inverter_temp_3_pin,
                driver_temperature_offset,
                driver_temperature_slope,
                &driver_temperature[2],
                driver_temperature_filter[2],
            },
            {
                Pinout::inverter_temp_4_pin,
                driver_temperature_offset,
                driver_temperature_slope,
                &driver_temperature[3],
                driver_temperature_filter[3],
            },
        };

   public:
    Temperature() = default;

    void read_motor_temperature();
    void read_driver_temperature();

    float *get_motor_temperature(uint8_t index);
    float *get_driver_temperature(uint8_t index);
};

};  // namespace BCU::Sensors