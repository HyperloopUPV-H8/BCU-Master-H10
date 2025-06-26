#pragma once

#include "ST-LIB.hpp"

namespace BCU::Communication {

template <uint16_t id>
class PolledOrder {
    HeapOrder order;

    static inline volatile bool has_been_received_flag{false};
    static void on_receive() { has_been_received_flag = true; }

   public:
    template <typename... Types>
    PolledOrder(Types *...values) : order{id, on_receive, values...} {}

    static bool has_been_received() { return has_been_received_flag; }
    static void clear_receive_flag() { has_been_received_flag = false; }
};

class Ethernet {
   public:
    inline static const IPV4 local_ip{"192.168.2.17"};
    inline static const MAC local_mac{"00:80:E1:00:02:17"};

    inline static const IPV4 master_ip{"192.168.0.9"};
    inline static const uint16_t master_data_port{50400};
    inline static const uint16_t master_command_port{50501};

    inline static const IPV4 lcu_ip{"192.168.1.4"};
    inline static const uint16_t lcu_command_source_port{50502};
    inline static const uint16_t lcu_command_destination_port{50503};

    PolledOrder<21700> disable_inverter_order;

    PolledOrder<21701> stop_control_order;

    PolledOrder<21702> precharge_filter_order;

    PolledOrder<21703> test_pwm_order{
        &test_pwm_parameters.duty_cycle_u,
        &test_pwm_parameters.duty_cycle_v,
        &test_pwm_parameters.duty_cycle_w,
    };

    PolledOrder<21704> test_space_vector_order{
        &space_vector_parameters.modulation_index,
        &space_vector_parameters.modulation_frequency_hz,
    };

    PolledOrder<21705> emulate_current_control_order{
        &emulated_current_control_parameters.current_ref,
        &emulated_current_control_parameters.linear_speed,
    };

    PolledOrder<21706> enable_current_control_order{
        &current_control_parameters.current_ref,
    };

    PolledOrder<21707> enable_speed_control_order{
        &speed_control_parameters.speed_ref,
    };

    PolledOrder<21708> fix_dc_link_voltage_order{
        &fix_dc_link_voltage_parameters.dc_link_voltage,
    };

    PolledOrder<21709> unfix_dc_link_voltage_order;

    PolledOrder<21710> configure_commutation_order{
        &configure_commutation_parameters.commutation_frequency_hz,
        &configure_commutation_parameters.dead_time_ns,
    };
    PolledOrder<21711> enable_booster;

    struct {
        double duty_cycle_u{0.0f};
        double duty_cycle_v{0.0f};
        double duty_cycle_w{0.0f};
    } test_pwm_parameters;

    struct {
        double modulation_index{0.0f};
        double modulation_frequency_hz{0.0f};
    } space_vector_parameters;

    struct {
        double current_ref{0.0f};
        double linear_speed{0.0f};
    } emulated_current_control_parameters;

    struct {
        double current_ref{0.0f};
    } current_control_parameters;

    struct {
        double speed_ref{0.0f};
    } speed_control_parameters;

    struct {
        double dc_link_voltage{0.0f};
    } fix_dc_link_voltage_parameters;

    struct {
        uint32_t commutation_frequency_hz{0};
        uint16_t dead_time_ns{0};
    } configure_commutation_parameters;

    struct CommutationDetailsData {
        double *duty_cycle_u;
        double *duty_cycle_v;
        double *duty_cycle_w;
        uint32_t *commutation_frequency_hz;
        uint16_t *dead_time_ns;
    };

    struct ClarkeParkTransformData {
        double *measured_current_d;
        double *measured_current_q;
        double *measured_current_alpha;
        double *measured_current_beta;
        double *measured_current_u;
        double *measured_current_v;
        double *measured_current_w;
        double *electrical_angle;
    };

    struct InverseClarkeParkTransformData {
        double *target_voltage_d;
        double *target_voltage_q;
        double *target_voltage_alpha;
        double *target_voltage_beta;
        double *target_voltage_u;
        double *target_voltage_v;
        double *target_voltage_w;
        double *dc_link_voltage;
    };

    struct ControlInformationData {
        double *reference_current_d;
        double *reference_current_q;
        double *reference_speed;
        double *error_current_d;
        double *error_current_q;
        double *error_speed;
    };

    struct CurrentSenseData {
        double *measured_current_u_1;
        double *measured_current_v_1;
        double *measured_current_w_1;
        double *measured_current_u_2;
        double *measured_current_v_2;
        double *measured_current_w_2;
        double *measured_current_u_3;
        double *measured_current_v_3;
        double *measured_current_w_3;
        double *measured_current_u_4;
        double *measured_current_v_4;
        double *measured_current_w_4;
    };

    struct EncoderSenseData {
        double *speetec_1_position;
        double *speetec_1_velocity;
        double *speetec_1_acceleration;
        double *speetec_2_position;
        double *speetec_2_velocity;
        double *speetec_2_acceleration;
        double *speetec_3_position;
        double *speetec_3_velocity;
        double *speetec_3_acceleration;
        double *speetec_position;
        double *speetec_velocity;
        double *speetec_acceleration;
    };

    struct MotorDriverSenseData {
        double *driver_1_dc_link_voltage;
        bool *driver_1_ready;
        bool *driver_1_fault;
        double *driver_2_dc_link_voltage;
        bool *driver_2_ready;
        bool *driver_2_fault;
        double *driver_3_dc_link_voltage;
        bool *driver_3_ready;
        bool *driver_3_fault;
        double *driver_4_dc_link_voltage;
        bool *driver_4_ready;
        bool *driver_4_fault;
    };

    struct TemperatureSenseData {
        float *motor_1_temperature;
        float *motor_2_temperature;
        float *motor_3_temperature;
        float *motor_4_temperature;
        float *driver_1_temperature;
        float *driver_2_temperature;
        float *driver_3_temperature;
        float *driver_4_temperature;
    };

    struct StateData {
        StateMachine::state_id *master_general_state;
        StateMachine::state_id *master_operational_state;
        StateMachine::state_id *slave_general_state;
        StateMachine::state_id *slave_operational_state;
    };

   private:
    HeapPacket commutation_details_packet;
    HeapPacket clarke_park_transform_packet;
    HeapPacket inverse_clarke_park_transform_packet;
    HeapPacket control_information_packet;
    HeapPacket current_sense_packet;
    HeapPacket encoder_sense_packet;
    HeapPacket motor_driver_sense_packet;
    HeapPacket temperature_sense_packet;
    HeapPacket state_packet;

    DatagramSocket master_data;
    ServerSocket master_command;

    Socket lcu_command;

   public:
    Ethernet(
        const CommutationDetailsData &commutation_details,
        const ClarkeParkTransformData &clarke_park_transform,
        const InverseClarkeParkTransformData &inverse_clarke_park_transform,
        const ControlInformationData &control_information,
        const CurrentSenseData &current_sense,
        const EncoderSenseData &encoder_sense,
        const MotorDriverSenseData &motor_driver_sense,
        const TemperatureSenseData &temperature_sense,
        const StateData &state_data);

    bool send_commutation_details();
    bool send_clarke_park_transform();
    bool send_inverse_clarke_park_transform();
    bool send_control_information();
    bool send_current_sense();
    bool send_encoder_sense();
    bool send_motor_driver_sense();
    bool send_temperature_sense();
    bool send_state();

    bool is_connected();
};

};  // namespace BCU::Communication
