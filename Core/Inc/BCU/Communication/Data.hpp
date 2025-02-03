#pragma once

#include "BCU/Communication/FDCAN.hpp"
#include "CMS-LIB.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication::Data {

inline Packet *generate_module_state_packet(uint16_t id, uint8_t module_id) {
    return new StackPacket(
        id,
        &Communication::FDCAN::supercaps.system.strings[1][module_id]
             .module_voltage.volts,
        &Communication::FDCAN::supercaps.system.strings[1][module_id]
             .live_count);
}

};  // namespace BCU::Communication::Data