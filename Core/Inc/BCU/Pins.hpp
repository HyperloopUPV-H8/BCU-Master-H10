#pragma once

#include "BCU/Config.hpp"
#include "ST-LIB.hpp"

#define CONTACTOR_1 PF2
#define CONTACTOR_2 PF1
#define CONTACTOR_3 PE6

namespace BCU::Pins {
inline Pin& ContactorHighPin{CONTACTOR_3};
inline Pin& ContactorPrechargePin{CONTACTOR_2};
inline Pin& ContactorActiveDischargePin{CONTACTOR_1};
#if USING_CONTACTOR_LOW
inline Pin& ContactorLowPin{PA1};
#endif

}  // namespace BCU::Pins