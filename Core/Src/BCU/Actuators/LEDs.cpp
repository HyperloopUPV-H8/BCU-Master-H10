#include "BCU/Actuators/LEDs.hpp"

using namespace BCU::Actuators;

LEDs::LEDs(Pin& led_operational_pin, Pin& led_fault_pin, Pin& led_can_pin,
           Pin& led_flash_pin, Pin& led_sleep_pin)
    : led_description(
          {{LED::Operational, Description{DigitalOutput(led_operational_pin)}},
           {LED::Fault, Description{DigitalOutput(led_fault_pin)}},
           {LED::Can, Description{DigitalOutput(led_can_pin)}},
           {LED::Flash, Description{DigitalOutput(led_flash_pin)}},
           {LED::Sleep, Description{DigitalOutput(led_sleep_pin)}}}) {}

inline void LEDs::clear_led_requests(Description& description) {
    if (description.blinking_alarm_id != undefined_id) {
        Time::unregister_low_precision_alarm(description.blinking_alarm_id);
        description.blinking_alarm_id = undefined_id;
    }

    if (description.quick_toggle_trigger_id != undefined_id) {
        Time::cancel_timeout(description.quick_toggle_trigger_id);
        description.output.toggle();
        description.quick_toggle_trigger_id = undefined_id;
    }
}

void LEDs::blink_led(LED led, uint32_t half_period_ms) {
    Description& description = led_description[led];

    clear_led_requests(description);

    description.blinking_alarm_id = Time::register_low_precision_alarm(
        half_period_ms, [&]() { description.output.toggle(); });
}

void LEDs::quick_toggle_led(LED led, uint32_t toggle_ms) {
    Description& description = led_description[led];

    clear_led_requests(description);

    description.output.toggle();
    description.quick_toggle_trigger_id = Time::set_timeout(toggle_ms, [&]() {
        description.quick_toggle_trigger_id = undefined_id;
        description.output.toggle();
    });
}

void LEDs::continuous_on_led(LED led) {
    Description& description = led_description[led];

    clear_led_requests(description);

    description.output.turn_on();
}

void LEDs::continuous_off_led(LED led) {
    Description& description = led_description[led];

    clear_led_requests(description);

    description.output.turn_off();
}

void LEDs::signal_connecting() { blink_led(LED::Operational); }

void LEDs::signal_operational() { continuous_on_led(LED::Operational); }

void LEDs::signal_fault() {
    continuous_off_led(LED::Operational);
    continuous_on_led(LED::Fault);
}

void LEDs::signal_can_started() { continuous_on_led(LED::Can); }

void LEDs::signal_can_transmit() { quick_toggle_led(LED::Can); }