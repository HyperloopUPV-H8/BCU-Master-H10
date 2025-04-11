#include "BCU/Communication/CAN.hpp"

namespace BCU::Communication {

CAN::CAN(Communication::SPI &spi)
    : can_id(FDCAN::inscribe<
             CANBitRatesSpeed::CAN_500_kbit, CANFormat::CAN_NORMAL_FORMAT,
             CANIdentifier::CAN_11_BIT_IDENTIFIER, CANMode::CAN_MODE_NORMAL>(
          FDCAN::fdcan1)),
      spi(spi) {}

void CAN::start() {}

void CAN::update() {
    if (FDCAN::read(can_id, &last_read)) {
        switch (last_read.identifier) {
            case 0x701:
                spi.start_test_pwm((float)last_read.rx_data[0],
                                   (float)last_read.rx_data[1],
                                   (float)last_read.rx_data[2]);
                break;
            case 0x702:
                spi.start_emulated_movement((float)last_read.rx_data[0],
                                            (float)last_read.rx_data[1],
                                            (float)last_read.rx_data[2]);
                break;
            case 0x703:
                spi.start_current_control((float)last_read.rx_data[0],
                                          (float)last_read.rx_data[1]);
                break;
            case 0x704:
                spi.start_velocity_control((float)last_read.rx_data[0]);
                break;
            case 0x705:
                spi.change_commutation_settings((uint32_t)last_read.rx_data[0],
                                                (uint32_t)last_read.rx_data[1]);
                break;
            case 0x706:
                spi.force_dc_link((float)last_read.rx_data[0]);
                break;
            case 0x707:
                spi.unlock_dc_link();
                break;
            case 0x708:
                spi.stop_control();
                break;
        }
    }
}

}  // namespace BCU::Communication
