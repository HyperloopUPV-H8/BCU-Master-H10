#pragma once

#include "BCU/Communication/SPI.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication {

class CAN {
    uint8_t can_id;

    Communication::SPI &spi;

    FDCAN::Packet last_read{};

   public:
    CAN(Communication::SPI &spi);

    void start();
    void update();
};

};  // namespace BCU::Communication