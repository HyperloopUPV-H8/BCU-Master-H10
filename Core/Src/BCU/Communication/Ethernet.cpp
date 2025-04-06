#include "BCU/Communication/Ethernet.hpp"

namespace BCU::Communication {

Ethernet::Ethernet(StateMachine::state_id *master_general_state,
                   StateMachine::state_id *master_operational_state,
                   StateMachine::state_id *slave_general_state,
                   StateMachine::state_id *slave_operational_state)
    : control_station_tcp(local_ip, tcp_server_port),
      control_station_udp(local_ip, udp_port, control_station_ip, udp_port),
      master_general_state(master_general_state),
      master_operational_state(master_operational_state),
      slave_general_state(slave_general_state),
      slave_operational_state(slave_operational_state) {}

void Ethernet::send_board_state() {
    control_station_udp.send_packet(board_state);
}

bool Ethernet::is_connected() { return control_station_tcp.is_connected(); }

};  // namespace BCU::Communication
