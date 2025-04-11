#include "BCU/Communication/CAN.hpp"

namespace BCU::Communication {

CAN::CAN()
    : can_id(FDCAN::inscribe<
             CANBitRatesSpeed::CAN_500_kbit, CANFormat::CAN_NORMAL_FORMAT,
             CANIdentifier::CAN_11_BIT_IDENTIFIER, CANMode::CAN_MODE_NORMAL>(
          FDCAN::fdcan1)) {}

void CAN::start() {}

void CAN::update() {
    if (FDCAN::read(can_id, &last_read)) {
        switch (last_read.identifier) {
            case start_test_pwm_id:
                has_received_start_test_pwm = true;
                requested_duty_cycle_u = (float)last_read.rx_data[0];
                requested_duty_cycle_v = (float)last_read.rx_data[1];
                requested_duty_cycle_w = (float)last_read.rx_data[2];
                break;
            case start_emulated_movement_id:
                has_received_start_emulated_movement = true;
                requested_d_current_reference = (float)last_read.rx_data[0];
                requested_q_current_reference = (float)last_read.rx_data[1];
                requested_angular_velocity = (float)last_read.rx_data[2];
                break;
            case start_current_control_id:
                has_received_start_current_control = true;
                break;
            case start_velocity_control_id:
                has_received_start_velocity_control = true;
                break;
            case change_commutation_settings_id:
                has_received_change_commutation_settings = true;
                break;
            case force_dc_link_id:
                has_received_force_dc_link = true;
                break;
            case unlock_dc_link_id:
                has_received_unlock_dc_link = true;
                break;
            case stop_control_id:
                has_received_stop_control = true;
                break;
        }
    }
}

}  // namespace BCU::Communication
