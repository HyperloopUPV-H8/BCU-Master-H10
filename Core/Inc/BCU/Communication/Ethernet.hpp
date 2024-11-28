#pragma once

#include "BCU/Communication/ModuleCAN.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication {
class Ethernet {
   private:
    /*********************
     * Socket Definition *
     *********************/

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

    /*********************
     * Packet Definition *
     *********************/

    static Packet *Packets[1];

    constexpr static uint16_t Module1CellsOverviewPacketId{999};

    static void initialize_data() {
        CMS::Module &module1{ModuleCAN::strings[1].modules[1]};

        Packets[0] = new StackPacket(
            Module1CellsOverviewPacketId, &module1.module_voltage,
            &module1.max_cell_voltage, &module1.mean_cell_voltage,
            &module1.min_cell_voltage);
    }

    /********************
     * Order Definition *
     ********************/

    static Order *Orders[4];

    constexpr static uint16_t OpenContactorsOrderId{900};
    constexpr static uint16_t CloseMainCircuitOrderId{901};
    constexpr static uint16_t CloseActiveDischargeOrderId{902};
    constexpr static uint16_t RequestDataToModule1OrderId{903};

    static void initialize_orders() {
        Orders[0] =
            new StackOrder(OpenContactorsOrderId, open_contactors_callback);
        Orders[1] = new StackOrder(CloseMainCircuitOrderId,
                                   close_main_circuit_callback);
        Orders[2] = new StackOrder(CloseActiveDischargeOrderId,
                                   close_active_discharge_callback);
        Orders[3] =
            new StackOrder(RequestDataToModule1OrderId, request_data_callback);
    }

    static void open_contactors_callback() { open_contactors_received = true; }
    static void close_main_circuit_callback() {
        close_main_circuit_received = true;
    }
    static void close_active_discharge_callback() {
        close_active_discharge_received = true;
    }
    static void request_data_callback() { request_data_received = true; }

   public:
    static bool open_contactors_received;
    static bool close_main_circuit_received;
    static bool close_active_discharge_received;

    static bool request_data_received;

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
