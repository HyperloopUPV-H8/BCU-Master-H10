#pragma once

#include "BCU/Communication/Data.hpp"
#include "BCU/Communication/Orders.hpp"
#include "CMS-LIB.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication {
class Ethernet {
   private:
    /*********************
     * Socket Definition *
     *********************/

    inline const static IPV4 LocalIP{"192.168.0.15"};
    inline const static IPV4 BackendIP{"192.168.0.9"};

    /*inline const static IPV4 LocalIP{"127.0.0.1"};
    inline const static IPV4 BackendIP{"127.0.0.1"}; Test use */

    inline const static uint32_t TCPServerPort{50500};
    inline const static uint32_t TCPClientPort{50401};
    inline const static uint32_t UDPPort{50400};

    inline static ServerSocket *ControlStationOrders{nullptr};
    inline static DatagramSocket *ControlStationData{nullptr};

    static void initialize_sockets() {
        ControlStationOrders = new ServerSocket(LocalIP, TCPServerPort);
        ControlStationData =
            new DatagramSocket(LocalIP, UDPPort, BackendIP, UDPPort);
    }

    /*********************
     * Packet Definition *
     *********************/

    inline static std::array<Packet *, 4> Packets{};

    static void initialize_data() {
        Packets[0] = Data::generate_module_state_packet(999, 1);
        Packets[1] = Data::generate_module_state_packet(998, 2);
        Packets[2] = Data::generate_module_state_packet(997, 3);
        Packets[3] = new StackPacket(
            996, &Communication::FDCAN::supercaps.system.total_voltage_volts);
    }

    /********************
     * Order Definition *
     ********************/

    static void initialize_orders() {
        Orders::RestartSupercaps();
        Orders::OpenContactors();
        Orders::CloseContactors();
    }

   public:
    static void initialize() {
        initialize_sockets();
        initialize_orders();
        initialize_data();
    }

    static bool is_connected() { return ControlStationOrders->is_connected(); }

    static bool update() {
        Orders::OpenContactors::update();
        Orders::CloseContactors::update();
        Orders::RestartSupercaps::update();

        return true;
    }

    static void send_supercaps_data() {
        ControlStationData->send_packet(*Packets[0]);
        ControlStationData->send_packet(*Packets[1]);
        ControlStationData->send_packet(*Packets[2]);
        ControlStationData->send_packet(*Packets[3]);
    }
};
}  // namespace BCU::Communication
