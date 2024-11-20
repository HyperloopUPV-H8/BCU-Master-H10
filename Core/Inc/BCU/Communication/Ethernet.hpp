#pragma once

#include "ST-LIB.hpp"

namespace BCU::Communication {
class Ethernet {
   private:
    const static IPV4 LocalIP;
    const static IPV4 BackendIP;

    const static uint32_t TCPServerPort;
    const static uint32_t TCPClientPort;
    const static uint32_t UDPPort;

    static ServerSocket *ControlStationOrders;
    static DatagramSocket *ControlStationData;

    static void initialize_sockets() {
        ControlStationOrders = new ServerSocket(LocalIP, TCPServerPort);
        ControlStationData =
            new DatagramSocket(LocalIP, UDPPort, BackendIP, UDPPort);
    }

    static Order *Orders[3];

    constexpr static uint16_t OpenContactorsOrderId{900};
    constexpr static uint16_t CloseMainCircuitOrderId{901};
    constexpr static uint16_t CloseActiveDischargeOrderId{902};

    static void initialize_orders() {
        Orders[0] =
            new StackOrder<0>(OpenContactorsOrderId, open_contactors_callback);
        Orders[1] = new StackOrder<0>(CloseMainCircuitOrderId,
                                      close_main_circuit_callback);
        Orders[2] = new StackOrder<0>(CloseActiveDischargeOrderId,
                                      close_active_discharge_callback);
    }

    static void open_contactors_callback() { open_contactors_received = true; }
    static void close_main_circuit_callback() {
        close_main_circuit_received = true;
    }
    static void close_active_discharge_callback() {
        close_active_discharge_received = true;
    }

   public:
    static bool open_contactors_received;
    static bool close_main_circuit_received;
    static bool close_active_discharge_received;

    static void reset_contactor_orders_received() {
        open_contactors_received = false;
        close_main_circuit_received = false;
        close_active_discharge_received = false;
    }

    static void initialize() {
        initialize_sockets();
        initialize_orders();
    }

    static bool is_connected() { return ControlStationOrders->is_connected(); }
};
}  // namespace BCU::Communication
