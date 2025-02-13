#pragma once

#include "BCU/Config.hpp"
#include "ST-LIB.hpp"

#define CONTACTOR_1 PF2
#define CONTACTOR_2 PF1
#define CONTACTOR_3 PE6

#define LED_OPERATIONAL PG8
#define LED_FAULT PG7
#define LED_CAN PG6
#define LED_FLASH PG5
#define LED_SLEEP PG4

namespace BCU::Pins {
inline Pin& contactor_high_pin{CONTACTOR_3};
inline Pin& contactor_precharge_pin{CONTACTOR_2};
inline Pin& contactor_supercaps_discharge_pin{CONTACTOR_1};
#if USING_CONTACTOR_LOW_SIDE
inline Pin& contactor_low_pin{PA1};
#endif
#if USING_CONTACTOR_SUPERCAPS_CHARGE
inline Pin& contactor_supercaps_charge_pin{PA2};
#endif
#if USING_CONTACTOR_INVERSOR_DISCHARGE
inline Pin& contactor_inversor_discharge_pin{PA3};
#endif

inline Pin& led_operational_pin{LED_OPERATIONAL};
inline Pin& led_fault_pin{LED_FAULT};
inline Pin& led_can_pin{LED_CAN};
inline Pin& led_flash_pin{LED_FLASH};
inline Pin& led_sleep_pin{LED_SLEEP};

}  // namespace BCU::Pins