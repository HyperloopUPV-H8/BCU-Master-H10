#include "BCU/Communication/SPI.hpp"

namespace BCU::Communication {

SPI::SPI(::SPI::Peripheral &peripheral, StateMachine::state_id *general_state,
         StateMachine::state_id *operational_state)
    : spi_id(::SPI::inscribe(peripheral)),
      state_order(Shared::Communication::create_state_order(
          general_state, operational_state, &slave_general_state,
          &slave_operational_state)) {}

void SPI::update() { ::SPI::Order_update(); }

bool SPI::sync_state() {
    return ::SPI::master_transmit_Order(spi_id, state_order);
}

};  // namespace BCU::Communication
