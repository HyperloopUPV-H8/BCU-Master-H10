#include "BCU/Communication/FDCAN.hpp"

uint8_t BCU::Communication::FDCAN::fdcan_id{};

inline constexpr uint8_t DLC_to_length(const FDCAN::DLC dlc) noexcept {
    switch (dlc) {
        case FDCAN::DLC::BYTES_0:
            return 0;
        case FDCAN::DLC::BYTES_1:
            return 1;
        case FDCAN::DLC::BYTES_2:
            return 2;
        case FDCAN::DLC::BYTES_3:
            return 3;
        case FDCAN::DLC::BYTES_4:
            return 4;
        case FDCAN::DLC::BYTES_5:
            return 5;
        case FDCAN::DLC::BYTES_6:
            return 6;
        case FDCAN::DLC::BYTES_7:
            return 7;
        case FDCAN::DLC::BYTES_8:
            return 8;
        case FDCAN::DLC::BYTES_12:
            return 12;
        case FDCAN::DLC::BYTES_16:
            return 16;
        case FDCAN::DLC::BYTES_20:
            return 20;
        case FDCAN::DLC::BYTES_24:
            return 24;
        case FDCAN::DLC::BYTES_32:
            return 32;
        case FDCAN::DLC::BYTES_48:
            return 48;
        case FDCAN::DLC::BYTES_64:
            return 64;
        default:
            return 0;
    }
}

inline constexpr FDCAN::DLC length_to_DLC(const uint8_t length) noexcept {
    switch (length) {
        case 0:
            return FDCAN::DLC::BYTES_0;
        case 1:
            return FDCAN::DLC::BYTES_1;
        case 2:
            return FDCAN::DLC::BYTES_2;
        case 3:
            return FDCAN::DLC::BYTES_3;
        case 4:
            return FDCAN::DLC::BYTES_4;
        case 5:
            return FDCAN::DLC::BYTES_5;
        case 6:
            return FDCAN::DLC::BYTES_6;
        case 7:
            return FDCAN::DLC::BYTES_7;
        case 8:
            return FDCAN::DLC::BYTES_8;
        case 12:
            return FDCAN::DLC::BYTES_12;
        case 16:
            return FDCAN::DLC::BYTES_16;
        case 20:
            return FDCAN::DLC::BYTES_20;
        case 24:
            return FDCAN::DLC::BYTES_24;
        case 32:
            return FDCAN::DLC::BYTES_32;
        case 48:
            return FDCAN::DLC::BYTES_48;
        case 64:
            return FDCAN::DLC::BYTES_64;
        default:
            return FDCAN::DLC::DEFAULT;
    }
}

void BCU::Communication::FDCAN::update() {
    using ::FDCAN;

    static FDCAN::Packet packet{};
    static CMS::Messages::Packet cms_packet{};

    if (!FDCAN::read(fdcan_id, &packet)) return;

    cms_packet.id = packet.identifier;
    cms_packet.length = DLC_to_length(packet.data_length);
    cms_packet.data = packet.rx_data;

    if (!ModuleCAN::decode(cms_packet)) return;
}

bool BCU::Communication::FDCAN::request_data_to(uint8_t module,
                                                CMS::DataRequestFlags tx_config,
                                                CMS::TxCycle tx_cycle) {
    using ::FDCAN;

    static CMS::Messages::Packet cms_packet{};

    if (!ModuleCAN::encode_data_request(1, module, tx_config, tx_cycle,
                                        cms_packet))
        return false;

    bool result =
        FDCAN::transmit(fdcan_id, cms_packet.id, (char *)cms_packet.data.data(),
                        length_to_DLC(cms_packet.length));
    return result;
}

bool BCU::Communication::FDCAN::request_all_data(
    CMS::DataRequestFlags tx_config, CMS::TxCycle tx_cycle) {
    using ::FDCAN;

    static CMS::Messages::Packet cms_packet{};

    if (!ModuleCAN::encode_data_request(tx_config, tx_cycle, cms_packet))
        return false;

    return FDCAN::transmit(fdcan_id, cms_packet.id,
                           (char *)cms_packet.data.data(),
                           length_to_DLC(cms_packet.length));
}

bool BCU::Communication::FDCAN::request_service_to(
    uint8_t module, CMS::ServiceCommand command) {
    using ::FDCAN;

    static CMS::Messages::Packet cms_packet{};

    if (!ModuleCAN::encode_service_request(1, module, command, cms_packet))
        return false;

    return FDCAN::transmit(fdcan_id, cms_packet.id,
                           (char *)cms_packet.data.data(),
                           length_to_DLC(cms_packet.length));
}

bool BCU::Communication::FDCAN::request_all_service(
    CMS::ServiceCommand command) {
    using ::FDCAN;

    static CMS::Messages::Packet cms_packet{};

    if (!ModuleCAN::encode_service_request(1, command, cms_packet))
        return false;

    return FDCAN::transmit(fdcan_id, cms_packet.id,
                           (char *)cms_packet.data.data(),
                           length_to_DLC(cms_packet.length));
}

bool BCU::Communication::FDCAN::request_restart_to(uint8_t module,
                                                   CMS::RestartMode mode) {
    using ::FDCAN;

    static CMS::Messages::Packet cms_packet{};

    if (!ModuleCAN::encode_restart_request(1, module, mode, cms_packet))
        return false;

    return FDCAN::transmit(fdcan_id, cms_packet.id,
                           (char *)cms_packet.data.data(),
                           length_to_DLC(cms_packet.length));
}

bool BCU::Communication::FDCAN::request_all_restart() {
    using ::FDCAN;

    static CMS::Messages::Packet cms_packet{};

    if (!ModuleCAN::encode_restart_request(cms_packet)) return false;

    return FDCAN::transmit(fdcan_id, cms_packet.id,
                           (char *)cms_packet.data.data(),
                           length_to_DLC(cms_packet.length));
}
