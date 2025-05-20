#pragma once

#include "ST-LIB.hpp"

namespace BCU::Communication {

template <uint16_t id, typename... Types>
class PolledOrder {
    HeapOrder order;

    static volatile bool has_been_received{false};
    static void on_receive() { has_been_received = true; }

   public:
    PolledOrder() : order(id, on_receive) {}
    PolledOrder(Types &...values) : order(id, on_receive, values...) {}

    static bool has_been_received() { return has_been_received; }
    static void clear_receive_flag() { has_been_received = false; }
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

    PolledOrder<21703, float, float, float> test_pwm_order{
        test_pwm_parameters.duty_cycle_u, test_pwm_parameters.duty_cycle_v,
        test_pwm_parameters.duty_cycle_w};

    PolledOrder<21704, float, float, float> test_space_vector_order{
        space_vector_parameters.modulation_index,
        space_vector_parameters.modulation_frequency_hz,
        space_vector_parameters.linear_speed};

    PolledOrder<21705, float, float> emulate_current_control_order{
        emulated_current_control_parameters.current_ref,
        emulated_current_control_parameters.linear_speed};

    PolledOrder<21706, float> enable_current_control_order{
        current_control_parameters.current_ref};

    PolledOrder<21707, float> enable_speed_control_order{
        speed_control_parameters.speed_ref};

    PolledOrder<21708, float> fix_dc_link_voltage_order{
        fix_dc_link_voltage_parameters.dc_link_voltage};

    PolledOrder<21709> unfix_dc_link_voltage_order;

    PolledOrder<21710, uint32_t, uint16_t> configure_commutation_order{
        configure_commutation_parameters.commutation_frequency_hz,
        configure_commutation_parameters.dead_time_ns};

    struct {
        float duty_cycle_u{0.0f};
        float duty_cycle_v{0.0f};
        float duty_cycle_w{0.0f};
    } test_pwm_parameters;

    struct {
        float modulation_index{0.0f};
        float modulation_frequency_hz{0.0f};
        float linear_speed{0.0f};
    } space_vector_parameters;

    struct {
        float current_ref{0.0f};
        float linear_speed{0.0f};
    } emulated_current_control_parameters;

    struct {
        float current_ref{0.0f};
    } current_control_parameters;

    struct {
        float speed_ref{0.0f};
    } speed_control_parameters;

    struct {
        float dc_link_voltage{0.0f};
    } fix_dc_link_voltage_parameters;

    struct {
        uint32_t commutation_frequency_hz{0};
        uint16_t dead_time_ns{0};
    } configure_commutation_parameters;

    struct CommutationDetailsData {
        float *duty_cycle_u;
        float *duty_cycle_v;
        float *duty_cycle_w;
        uint32_t *commutation_frequency_hz;
        uint16_t *dead_time_ns;
    };

    struct ClarkeParkTransformData {
        float *measured_current_d;
        float *measured_current_q;
        float *measured_current_alpha;
        float *measured_current_beta;
        float *measured_current_u;
        float *measured_current_v;
        float *measured_current_w;
        float *electrical_angle;
    };

    struct InverseClarkeParkTransformData {
        float *target_voltage_d;
        float *target_voltage_q;
        float *target_voltage_alpha;
        float *target_voltage_beta;
        float *target_voltage_u;
        float *target_voltage_v;
        float *target_voltage_w;
        float *dc_link_voltage;
    };

    struct ControlInformationData {
        float *reference_current_d;
        float *reference_current_q;
        float *reference_speed;
        float *error_current_d;
        float *error_current_q;
        float *error_speed;
    };

    struct CurrentSenseData {
        float *measured_current_u_1;
        float *measured_current_v_1;
        float *measured_current_w_1;
        float *measured_current_u_2;
        float *measured_current_v_2;
        float *measured_current_w_2;
        float *measured_current_u_3;
        float *measured_current_v_3;
        float *measured_current_w_3;
        float *measured_current_u_4;
        float *measured_current_v_4;
        float *measured_current_w_4;
    };

    struct EncoderSenseData {
        float *speetec_1_position;
        float *speetec_1_velocity;
        float *speetec_1_acceleration;
        float *speetec_2_position;
        float *speetec_2_velocity;
        float *speetec_2_acceleration;
        float *speetec_3_position;
        float *speetec_3_velocity;
        float *speetec_3_acceleration;
        float *speetec_position;
        float *speetec_velocity;
        float *speetec_acceleration;
    };

    struct MotorDriverSenseData {
        float *driver_1_dc_link_voltage;
        bool *driver_1_ready;
        bool *driver_1_fault;
        float *driver_2_dc_link_voltage;
        bool *driver_2_ready;
        bool *driver_2_fault;
        float *driver_3_dc_link_voltage;
        bool *driver_3_ready;
        bool *driver_3_fault;
        float *driver_4_dc_link_voltage;
        bool *driver_4_ready;
        bool *driver_4_fault;
    };

    struct TemperatureSenseData {
        float *driver_1_temperature;
        float *motor_1_temperature;
        float *driver_2_temperature;
        float *motor_2_temperature;
        float *driver_3_temperature;
        float *motor_3_temperature;
        float *driver_4_temperature;
        float *motor_4_temperature;
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
        const TemperatureSenseData &temperature_sense);

    void send_commutation_details();
    void send_clarke_park_transform();
    void send_inverse_clarke_park_transform();
    void send_control_information();
    void send_current_sense();
    void send_encoder_sense();
    void send_motor_driver_sense();
    void send_temperature_sense();

    bool is_connected() const;
};

};  // namespace BCU::Communication
