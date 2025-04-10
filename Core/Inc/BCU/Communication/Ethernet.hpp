#pragma once

#include "ST-LIB.hpp"
#include "Shared/SPI.hpp"

namespace BCU::Communication {

class Ethernet {
   public:
    double requested_velocity_reference{0.0};
    double requested_d_current_reference{0.0};
    double requested_q_current_reference{0.0};
    double requested_angular_velocity{0.0};
    double requested_duty_cycle_u{0.0};
    double requested_duty_cycle_v{0.0};
    double requested_duty_cycle_w{0.0};
    float requested_dc_link_voltage{0.0f};
    uint32_t requested_commutation_frequency_hz{0};
    uint32_t requested_commutation_dead_time_ns{0};

    static bool has_requested_start_velocity_control;
    static bool has_requested_start_current_control;
    static bool has_requested_start_emulated_movement;
    static bool has_requested_start_test_pwm;
    static bool has_requested_stop_control;
    static bool has_requested_enable_booster;
    static bool has_requested_force_dc_link;
    static bool has_requested_unlock_dc_link;
    static bool has_requested_commutation_settings;

   private:
    static void on_start_velocity_control() {
        has_requested_start_velocity_control = true;
    }
    static void on_start_current_control() {
        has_requested_start_current_control = true;
    }
    static void on_start_emulated_movement() {
        has_requested_start_emulated_movement = true;
    }
    static void on_start_test_pwm() { has_requested_start_test_pwm = true; }
    static void on_stop_control() { has_requested_stop_control = true; }
    static void on_enable_booster() { has_requested_enable_booster = true; }
    static void on_force_dc_link() { has_requested_force_dc_link = true; }
    static void on_unlock_dc_link() { has_requested_unlock_dc_link = true; }
    static void on_commutation_settings() {
        has_requested_commutation_settings = true;
    }

    ServerSocket control_station_tcp;
    DatagramSocket control_station_udp;

    StateMachine::state_id *master_general_state;
    StateMachine::state_id *master_operational_state;
    StateMachine::state_id *slave_general_state;
    StateMachine::state_id *slave_operational_state;

    double *velocity_reference;
    double *velocity_error;
    double *u_current_measurement;
    double *v_current_measurement;
    double *w_current_measurement;
    double *electrical_angle;
    double *d_current_reference;
    double *d_current_measurement;
    double *d_current_error;
    double *q_current_reference;
    double *q_current_measurement;
    double *q_current_error;
    double *three_phase_unbalance;
    double *d_target_voltage;
    double *q_target_voltage;
    double *u_target_voltage;
    double *v_target_voltage;
    double *w_target_voltage;
    double *u_output_voltage;
    double *v_output_voltage;
    double *w_output_voltage;
    double *u_duty_cycle;
    double *v_duty_cycle;
    double *w_duty_cycle;
    double *angular_velocity;
    std::array<double *, 3> position;
    std::array<double *, 3> velocity;
    std::array<double *, 3> acceleration;
    std::array<Shared::Communication::Direction *, 3> direction;
    double *average_position;
    double *max_velocity;
    bool *is_detecting;
    std::array<float *, 4> dc_link_voltage;
    std::array<std::array<float *, 3>, 4> motor_phase_current;
    std::array<PinState *, 4> gate_driver_fault;
    std::array<PinState *, 4> gate_driver_ready;

    HeapPacket board_state{1700, master_general_state, master_operational_state,
                           slave_general_state, slave_operational_state};

    HeapPacket control_data{1701,
                            velocity_reference,
                            velocity_error,
                            u_current_measurement,
                            v_current_measurement,
                            w_current_measurement,
                            electrical_angle,
                            d_current_reference,
                            d_current_measurement,
                            d_current_error,
                            q_current_reference,
                            q_current_measurement,
                            q_current_error,
                            three_phase_unbalance,
                            d_target_voltage,
                            q_target_voltage,
                            u_target_voltage,
                            v_target_voltage,
                            w_target_voltage,
                            u_output_voltage,
                            v_output_voltage,
                            w_output_voltage,
                            u_duty_cycle,
                            v_duty_cycle,
                            w_duty_cycle,
                            angular_velocity};
    HeapPacket motor_driver{1702,
                            dc_link_voltage[0],
                            dc_link_voltage[1],
                            dc_link_voltage[2],
                            dc_link_voltage[3],
                            motor_phase_current[0][0],
                            motor_phase_current[0][1],
                            motor_phase_current[0][2],
                            motor_phase_current[1][0],
                            motor_phase_current[1][1],
                            motor_phase_current[1][2],
                            motor_phase_current[2][0],
                            motor_phase_current[2][1],
                            motor_phase_current[2][2],
                            motor_phase_current[3][0],
                            motor_phase_current[3][1],
                            motor_phase_current[3][2],
                            gate_driver_fault[0],
                            gate_driver_fault[1],
                            gate_driver_fault[2],
                            gate_driver_fault[3],
                            gate_driver_ready[0],
                            gate_driver_ready[1],
                            gate_driver_ready[2],
                            gate_driver_ready[3]};
    HeapPacket position_data{1703,
                             position[0],
                             position[1],
                             position[2],
                             velocity[0],
                             velocity[1],
                             velocity[2],
                             acceleration[0],
                             acceleration[1],
                             acceleration[2],
                             direction[0],
                             direction[1],
                             direction[2],
                             average_position,
                             max_velocity,
                             is_detecting};

    HeapOrder start_velocity_control{1799, on_start_velocity_control,
                                     &requested_velocity_reference};
    HeapOrder start_current_control{1798, on_start_current_control,
                                    &requested_d_current_reference,
                                    &requested_q_current_reference};
    HeapOrder start_emulated_movement{
        1797, on_start_emulated_movement, &requested_d_current_reference,
        &requested_q_current_reference, &requested_angular_velocity};
    HeapOrder start_test_pwm{1796, on_start_test_pwm, &requested_duty_cycle_u,
                             &requested_duty_cycle_v, &requested_duty_cycle_w};
    HeapOrder stop_control{1795, on_stop_control};
    HeapOrder enable_booster{1794, on_enable_booster};
    HeapOrder force_dc_link{1793, on_force_dc_link, &requested_dc_link_voltage};
    HeapOrder unlock_dc_link{1792, on_unlock_dc_link};
    HeapOrder commutation_settings{1791, on_commutation_settings,
                                   &requested_commutation_frequency_hz,
                                   &requested_commutation_dead_time_ns};

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