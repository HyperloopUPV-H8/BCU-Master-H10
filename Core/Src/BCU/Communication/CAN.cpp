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
                memcpy(&requested_duty_cycle_u, last_read.rx_data.data(),
                       sizeof(requested_duty_cycle_u));
                memcpy(
                    &requested_duty_cycle_v,
                    last_read.rx_data.data() + sizeof(requested_duty_cycle_u),
                    sizeof(requested_duty_cycle_v));
                memcpy(&requested_duty_cycle_w,
                       last_read.rx_data.data() +
                           sizeof(requested_duty_cycle_u) +
                           sizeof(requested_duty_cycle_v),
                       sizeof(requested_duty_cycle_w));
                has_received_start_test_pwm = true;
                break;
            case configure_commutation_parameters_id:
                memcpy(&requested_commutation_frequency_hz,
                       last_read.rx_data.data(),
                       sizeof(requested_commutation_frequency_hz));
                memcpy(&requested_dead_time_ns,
                       last_read.rx_data.data() +
                           sizeof(requested_commutation_frequency_hz),
                       sizeof(requested_dead_time_ns));
                has_received_configure_commutation_parameters = true;
                break;
            case stop_control_id:
                has_received_stop_control = true;
                break;
            case start_space_vector_id:
                memcpy(&requested_modulation_index, last_read.rx_data.data(),
                       sizeof(requested_modulation_index));
                memcpy(&requested_modulation_frequency_hz,
                       last_read.rx_data.data() +
                           sizeof(requested_modulation_index),
                       sizeof(requested_modulation_frequency_hz));
                has_received_start_space_vector = true;
                break;
            case fix_dc_link_voltage_id:
                memcpy(&requested_dc_link_voltage, last_read.rx_data.data(),
                       sizeof(requested_dc_link_voltage));
                has_received_fix_dc_link_voltage = true;
                break;
            case unfix_dc_link_voltage_id:
                has_received_unfix_dc_link_voltage = true;
                break;
        }
    }
}

void CAN::transmit_state(StateMachine::state_id master_general_state,
                         StateMachine::state_id master_nested_state,
                         StateMachine::state_id slave_general_state,
                         StateMachine::state_id slave_nested_state) {
    std::array<uint8_t, 4 * sizeof(StateMachine::state_id)> buffer;
    memcpy(buffer.data(), &master_general_state, sizeof(master_general_state));
    memcpy(buffer.data() + sizeof(master_general_state), &master_nested_state,
           sizeof(master_nested_state));
    memcpy(buffer.data() + 2 * sizeof(master_general_state),
           &slave_general_state, sizeof(slave_general_state));
    memcpy(buffer.data() + 3 * sizeof(master_general_state),
           &slave_nested_state, sizeof(slave_nested_state));
    FDCAN::transmit(can_id, state_id,
                    reinterpret_cast<const char *>(buffer.data()),
                    FDCAN::DLC::BYTES_4);
}

void CAN::transmit_control_parameters(float duty_cycle_u, float duty_cycle_v,
                                      float duty_cycle_w) {
    uint16_t encoded_duty_cycle_u{(uint16_t)(duty_cycle_u * 100.0f)};
    uint16_t encoded_duty_cycle_v{(uint16_t)(duty_cycle_v * 100.0f)};
    uint16_t encoded_duty_cycle_w{(uint16_t)(duty_cycle_w * 100.0f)};

    std::array<uint8_t, 3 * sizeof(uint16_t)> buffer;
    memcpy(buffer.data(), &encoded_duty_cycle_u, sizeof(encoded_duty_cycle_u));
    memcpy(buffer.data() + sizeof(encoded_duty_cycle_u), &encoded_duty_cycle_v,
           sizeof(encoded_duty_cycle_v));
    memcpy(buffer.data() + 2 * sizeof(encoded_duty_cycle_u),
           &encoded_duty_cycle_w, sizeof(encoded_duty_cycle_w));

    FDCAN::transmit(can_id, control_parameters_id,
                    reinterpret_cast<const char *>(buffer.data()),
                    FDCAN::DLC::BYTES_6);
}

void CAN::transmit_dc_link_voltage(float average_dc_link_voltage,
                                   float dc_link_voltage_1,
                                   float dc_link_voltage_2,
                                   float dc_link_voltage_3,
                                   float dc_link_voltage_4) {
    uint8_t encoded_voltage_1{(uint8_t)dc_link_voltage_1};
    uint8_t encoded_voltage_2{(uint8_t)dc_link_voltage_2};
    uint8_t encoded_voltage_3{(uint8_t)dc_link_voltage_3};
    uint8_t encoded_voltage_4{(uint8_t)dc_link_voltage_4};

    std::array<uint8_t, 4 * sizeof(uint8_t) + sizeof(float)> buffer;

    memcpy(buffer.data(), &average_dc_link_voltage, sizeof(float));
    memcpy(buffer.data() + 4, &encoded_voltage_1, sizeof(encoded_voltage_1));
    memcpy(buffer.data() + 5, &encoded_voltage_2, sizeof(encoded_voltage_2));
    memcpy(buffer.data() + 6, &encoded_voltage_3, sizeof(encoded_voltage_3));
    memcpy(buffer.data() + 7, &encoded_voltage_4, sizeof(encoded_voltage_4));

    FDCAN::transmit(can_id, dc_link_id,
                    reinterpret_cast<const char *>(buffer.data()),
                    FDCAN::DLC::BYTES_8);
}

}  // namespace BCU::Communication
