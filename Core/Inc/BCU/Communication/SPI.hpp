#pragma once

#include "ST-LIB.hpp"
#include "Shared/SPI.hpp"

namespace BCU::Communication {

class SPI {
   public:
    StateMachine::state_id slave_general_state;
    StateMachine::state_id slave_operational_state;

   private:
    uint8_t spi_id;

    SPIStackOrder *state_order;

   public:
    SPI(::SPI::Peripheral &peripheral, StateMachine::state_id *general_state,
        StateMachine::state_id *operational_state);

    void update();

    bool sync_state();
};

};  // namespace BCU::Communication
