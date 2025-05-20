#include "BCU/Communication/Ethernet.hpp"

namespace BCU::Communication {

Ethernet::Ethernet(
    const CommutationDetailsData &commutation_details,
    const ClarkeParkTransformData &clarke_park_transform,
    const InverseClarkeParkTransformData &inverse_clarke_park_transform,
    const ControlInformationData &control_information,
    const CurrentSenseData &current_sense,
    const EncoderSenseData &encoder_sense,
    const MotorDriverSenseData &motor_driver_sense,
    const TemperatureSenseData &temperature_sense)
    : commutation_details_packet{21711,
                                 commutation_details.duty_cycle_u,
                                 commutation_details.duty_cycle_v,
                                 commutation_details.duty_cycle_w,
                                 commutation_details.commutation_frequency_hz,
                                 commutation_details.dead_time_ns},
      clarke_park_transform_packet{
          21712,
          clarke_park_transform.measured_current_u,
          clarke_park_transform.measured_current_v,
          clarke_park_transform.measured_current_w,
          clarke_park_transform.measured_current_alpha,
          clarke_park_transform.measured_current_beta,
          clarke_park_transform.measured_current_d,
          clarke_park_transform.measured_current_q,
          clarke_park_transform.electrical_angle,
      },
      inverse_clarke_park_transform_packet{
          21713,
          inverse_clarke_park_transform.target_voltage_u,
          inverse_clarke_park_transform.target_voltage_v,
          inverse_clarke_park_transform.target_voltage_w,
          inverse_clarke_park_transform.target_voltage_alpha,
          inverse_clarke_park_transform.target_voltage_beta,
          inverse_clarke_park_transform.target_voltage_d,
          inverse_clarke_park_transform.target_voltage_q,
          inverse_clarke_park_transform.dc_link_voltage,
      },
      control_information_packet{
          21714,
          control_information.reference_current_d,
          control_information.reference_current_q,
          control_information.reference_speed,
          control_information.error_current_d,
          control_information.error_current_q,
          control_information.error_speed,
      },
      current_sense_packet{
          21715,
          current_sense.measured_current_u_1,
          current_sense.measured_current_v_1,
          current_sense.measured_current_w_1,
          current_sense.measured_current_u_2,
          current_sense.measured_current_v_2,
          current_sense.measured_current_w_2,
      },
      encoder_sense_packet{
          21716,
          encoder_sense.speetec_1_position,
          encoder_sense.speetec_1_velocity,
          encoder_sense.speetec_1_acceleration,
          encoder_sense.speetec_2_position,
          encoder_sense.speetec_2_velocity,
          encoder_sense.speetec_2_acceleration,
          encoder_sense.speetec_3_position,
          encoder_sense.speetec_3_velocity,
          encoder_sense.speetec_3_acceleration,
          encoder_sense.speetec_position,
          encoder_sense.speetec_velocity,
          encoder_sense.speetec_acceleration,
      },
      motor_driver_sense_packet{
          21717,
          motor_driver_sense.driver_1_dc_link_voltage,
          motor_driver_sense.driver_1_ready,
          motor_driver_sense.driver_1_fault,
          motor_driver_sense.driver_2_dc_link_voltage,
          motor_driver_sense.driver_2_ready,
          motor_driver_sense.driver_2_fault,
          motor_driver_sense.driver_3_dc_link_voltage,
          motor_driver_sense.driver_3_ready,
          motor_driver_sense.driver_3_fault,
          motor_driver_sense.driver_4_dc_link_voltage,
          motor_driver_sense.driver_4_ready,
          motor_driver_sense.driver_4_fault,
      },
      temperature_sense_packet{
          21718,
          temperature_sense.driver_1_temperature,
          temperature_sense.driver_2_temperature,
          temperature_sense.driver_3_temperature,
          temperature_sense.driver_4_temperature,
          temperature_sense.motor_1_temperature,
          temperature_sense.motor_2_temperature,
          temperature_sense.motor_3_temperature,
          temperature_sense.motor_4_temperature,
      },
      master_data{local_ip, master_data_port, master_ip, master_data_port},
      master_command{local_ip, master_command_port},
      lcu_command{local_ip, lcu_command_source_port, lcu_ip,
                  lcu_command_destination_port} {}

void Ethernet::send_commutation_details() {
    master_data.send_packet(commutation_details_packet);
}

void Ethernet::send_clarke_park_transform() {
    master_data.send_packet(clarke_park_transform_packet);
}

void Ethernet::send_inverse_clarke_park_transform() {
    master_data.send_packet(inverse_clarke_park_transform_packet);
}

void Ethernet::send_control_information() {
    master_data.send_packet(control_information_packet);
}

void Ethernet::send_current_sense() {
    master_data.send_packet(current_sense_packet);
}

void Ethernet::send_encoder_sense() {
    master_data.send_packet(encoder_sense_packet);
}

void Ethernet::send_motor_driver_sense() {
    master_data.send_packet(motor_driver_sense_packet);
}

void Ethernet::send_temperature_sense() {
    master_data.send_packet(temperature_sense_packet);
}

bool Ethernet::is_connected() {
    return master_command.is_connected() && lcu_command.is_connected();
}

};  // namespace BCU::Communication