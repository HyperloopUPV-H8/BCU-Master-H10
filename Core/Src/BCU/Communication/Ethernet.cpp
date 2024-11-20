#include "BCU/Communication/Ethernet.hpp"

const IPV4 BCU::Communication::Ethernet::LocalIP("192.168.0.15");
const IPV4 BCU::Communication::Ethernet::BackendIP("192.168.0.9");

const uint32_t BCU::Communication::Ethernet::TCPServerPort{50500};
const uint32_t BCU::Communication::Ethernet::TCPClientPort{50401};
const uint32_t BCU::Communication::Ethernet::UDPPort{50400};

ServerSocket *BCU::Communication::Ethernet::ControlStationOrders{};
DatagramSocket *BCU::Communication::Ethernet::ControlStationData{};

Order *BCU::Communication::Ethernet::Orders[3]{};

bool BCU::Communication::Ethernet::open_contactors_received{false};
bool BCU::Communication::Ethernet::close_main_circuit_received{false};
bool BCU::Communication::Ethernet::close_active_discharge_received{false};