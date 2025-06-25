#include "BCU/Communication/SPI.hpp"

namespace BCU::Communication {

SPI::SPI(::SPI::Peripheral &peripheral, StateMachine::state_id *general_state,
         StateMachine::state_id *operational_state)
    : spi_id(::SPI::inscribe(peripheral)),
      state_order(Shared::Communication::create_state_order(
          general_state, operational_state, &slave_general_state,
          &slave_operational_state)),
      encoder_order(Shared::Communication::create_encoder_order(
          &speetec_position, &speetec_velocity, &speetec_acceleration,
          &speetec_direction)),
      detailed_encoder_order(
          Shared::Communication::create_detailed_encoder_order(
              &speetec_1_position, &speetec_1_velocity, &speetec_1_acceleration,
              &speetec_direction_1, &speetec_2_position, &speetec_2_velocity,
              &speetec_2_acceleration, &speetec_direction_2)) {}

void SPI::update() { ::SPI::Order_update(); }

bool SPI::sync_state() {
    return ::SPI::master_transmit_Order(spi_id, state_order);
}

bool SPI::sync_encoder() {
    return ::SPI::master_transmit_Order(spi_id, encoder_order) &&
           ::SPI::master_transmit_Order(spi_id, detailed_encoder_order);
}

};  // namespace BCU::Communication
