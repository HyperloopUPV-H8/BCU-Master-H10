#include "BCU/Communication/SPI.hpp"

namespace BCU::Communication {

SPI::SPI(StateMachine::state_id *master_general_state,
         StateMachine::state_id *master_nested_state)
    : spi_id(::SPI::inscribe(::SPI::spi3)),
      master_general_state(master_general_state),
      master_nested_state(master_nested_state) {}

void SPI::start() { ::SPI::start(); }

void SPI::update() { ::SPI::Order_update(); }

void SPI::transmit_state() {
    ::SPI::master_transmit_Order(spi_id, state_order);
}

};  // namespace BCU::Communication
