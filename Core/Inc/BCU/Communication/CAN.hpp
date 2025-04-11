#pragma once

#include "BCU/Communication/SPI.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication {

class CAN {
   public:
    float requested_duty_cycle_u{0.0f};
    float requested_duty_cycle_v{0.0f};
    float requested_duty_cycle_w{0.0f};
    float requested_d_current_reference{0.0f};
    float requested_q_current_reference{0.0f};
    float requested_angular_velocity{0.0f};
    float requested_commutation_frequency_hz{0.0f};
    float requested_dead_time_ns{0.0f};

    bool has_received_start_test_pwm{false};
    bool has_received_start_emulated_movement{false};
    bool has_received_start_current_control{false};
    bool has_received_start_velocity_control{false};
    bool has_received_change_commutation_settings{false};
    bool has_received_force_dc_link{false};
    bool has_received_unlock_dc_link{false};
    bool has_received_stop_control{false};
    bool has_received_fault{false};

   private:
    constexpr static uint16_t start_test_pwm_id{0x721};
    constexpr static uint16_t start_emulated_movement_id{0x722};
    constexpr static uint16_t start_current_control_id{0x723};
    constexpr static uint16_t start_velocity_control_id{0x724};
    constexpr static uint16_t change_commutation_settings_id{0x725};
    constexpr static uint16_t force_dc_link_id{0x726};
    constexpr static uint16_t unlock_dc_link_id{0x727};
    constexpr static uint16_t stop_control_id{0x728};
    constexpr static uint16_t fault_id{0x729};

    uint8_t can_id;

    FDCAN::Packet last_read{};

   public:
    CAN();

    void start();
    void update();
};

};  // namespace BCU::Communication