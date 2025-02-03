#pragma once

#include "CMS-LIB.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication {
class FDCAN {
    static uint8_t fdcan_id;

    inline constexpr static uint8_t DLC_to_length(
        const ::FDCAN::DLC dlc) noexcept;

    inline constexpr static ::FDCAN::DLC length_to_DLC(
        const uint8_t length) noexcept;

    static bool transmit(CMS::Messages::CanPacket&& packet);

    static bool receive(CMS::Messages::CanPacket& packet);

   public:
    inline static CMS::Communications::ModuleCAN supercaps =
        CMS::Communications::ModuleCAN(receive, transmit);

    static void initialize() {
        using ::FDCAN;
        fdcan_id = FDCAN::inscribe(FDCAN::fdcan1);
    }

    static void start() {
        supercaps.start_module(1, 1);
        supercaps.start_module(1, 2);
        supercaps.start_module(1, 3);
    }

    static void restart() {
        supercaps.restart_module(1, 1);
        supercaps.restart_module(1, 2);
        supercaps.restart_module(1, 3);

        Time::set_timeout(500, start);
    }

    static void set_supercaps_data_rate(CMS::Types::TxCycle_t rate_ms) {
        supercaps.change_module_tx_cycle(1, 1, rate_ms);
        supercaps.change_module_tx_cycle(1, 2, rate_ms);
        supercaps.change_module_tx_cycle(1, 3, rate_ms);
    }

    static void update() { supercaps.update(); }
};

}  // namespace BCU::Communication