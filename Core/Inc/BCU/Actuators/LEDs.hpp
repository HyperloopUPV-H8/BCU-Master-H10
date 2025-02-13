#pragma once

#include "BCU/Pins.hpp"
#include "ST-LIB.hpp"

namespace BCU::Actuators {
class LEDs {
   private:
    constexpr static uint32_t blink_half_period_ms{500};
    constexpr static uint32_t quick_toggle_time_ms{100};
    constexpr static uint8_t undefined_id{0xFF};

    enum class LED {
        Operational = 0,
        Fault = 1,
        Can = 2,
        Flash = 3,
        Sleep = 4
    };

    struct Description {
        DigitalOutput output;
        uint8_t blinking_alarm_id{undefined_id};
        uint8_t quick_toggle_trigger_id{undefined_id};
    };

    std::unordered_map<LED, Description> led_description;

    inline void clear_led_requests(Description& description);

    void blink_led(LED led, uint32_t half_period_ms = blink_half_period_ms);

    void quick_toggle_led(LED led, uint32_t toggle_ms = quick_toggle_time_ms);

    void continuous_on_led(LED led);

    void continuous_off_led(LED led);

   public:
    LEDs(Pin& led_operational_pin = Pins::led_operational_pin,
         Pin& led_fault_pin = Pins::led_fault_pin,
         Pin& led_can_pin = Pins::led_can_pin,
         Pin& led_flash_pin = Pins::led_flash_pin,
         Pin& led_sleep_pin = Pins::led_sleep_pin);

    void signal_connecting();
    void signal_operational();
    void signal_fault();

    void signal_can_started();
    void signal_can_transmit();
};

};  // namespace BCU::Actuators