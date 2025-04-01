#pragma once

#include "ST-LIB.hpp"

namespace BCU::Communication {

class SPI {
    uint8_t spi_id;

    StateMachine::state_id *master_general_state;
    StateMachine::state_id *master_nested_state;

    SPIPacket<2, StateMachine::state_id, StateMachine::state_id>
        slave_state_packet{&slave_general_state, &slave_nested_state};
    SPIPacket<2, StateMachine::state_id, StateMachine::state_id>
        master_state_packet{master_general_state, master_nested_state};
    SPIStackOrder state_order{1, slave_state_packet, master_state_packet};

   public:
    StateMachine::state_id slave_general_state;
    StateMachine::state_id slave_nested_state;

    SPI(StateMachine::state_id *master_general_state,
        StateMachine::state_id *master_nested_state);

    void start();

    void update();

    void transmit_state();
};

}  // namespace BCU::Communication