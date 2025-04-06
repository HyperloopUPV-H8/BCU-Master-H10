#pragma once

#include "ST-LIB.hpp"

namespace BCU::Communication {

class Ethernet {
    ServerSocket control_station_tcp;
    DatagramSocket control_station_udp;

    StateMachine::state_id *master_general_state;
    StateMachine::state_id *master_operational_state;
    StateMachine::state_id *slave_general_state;
    StateMachine::state_id *slave_operational_state;

    HeapPacket board_state{1700, master_general_state, master_operational_state,
                           slave_general_state, slave_operational_state};

   public:
    inline static const IPV4 local_ip{"192.168.2.17"};
    inline static const IPV4 control_station_ip{"192.168.0.9"};

    inline static const uint16_t tcp_server_port{50500};
    inline static const uint16_t tcp_client_port{50401};
    inline static const uint16_t udp_port{50400};

    Ethernet(StateMachine::state_id *master_general_state,
             StateMachine::state_id *master_operational_state,
             StateMachine::state_id *slave_general_state,
             StateMachine::state_id *slave_operational_state);

    void send_board_state();

    bool is_connected();
};

};  // namespace BCU::Communication