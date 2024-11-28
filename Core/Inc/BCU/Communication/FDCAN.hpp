#pragma once

#include "BCU/Communication/ModuleCAN.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication {
class FDCAN {
   private:
    constexpr static uint8_t ModuleAmount{3};

    static uint8_t fdcan_id;

   public:
    static void initialize() {
        using ::FDCAN;
        fdcan_id = FDCAN::inscribe(FDCAN::fdcan1);
    }

    static void update();

    static bool request_data_to(uint8_t module, CMS::DataRequestFlags tx_config,
                                CMS::TxCycle tx_cycle);
    static bool request_all_data(CMS::DataRequestFlags tx_config,
                                 CMS::TxCycle tx_cycle);

    static bool request_service_to(uint8_t module, CMS::ServiceCommand command);
    static bool request_all_service(CMS::ServiceCommand command);

    static bool request_restart_to(uint8_t module, CMS::RestartMode mode);
    static bool request_all_restart();
};

}  // namespace BCU::Communication