#include "BCU/Pinout.hpp"

#define UART_TX_M_RX_S PD5
#define UART_RX_M_TX_S PD6

#define SPI_CS PD3
#define SPI_MOSI PC12
#define SPI_MISO PC11
#define SPI_CLK PC10

#define M_TXD PD1
#define M_RXD PD0
#define SLNT_Master PA8

#define LED_OPERATIONAL_M PG8
#define LED_FAULT_M PG7
#define LED_CAN_M PG6
#define LED_FLASH_M PG5
#define LED_SLEEP_M PG4

#define GPIO1 PD8
#define GPIO2 PD9
#define GPIO3 PD10
#define GPIO4 PD11
#define EXTI_M_GPIO5 PD5
#define EXTI_S_GPIO5 PG2

#define ENABLE_BUFFER_M_1 PE7
#define ENABLE_BUFFER_M_2 PE14
#define ENABLE_BUFFER_M_3 PE5

#define PT100_1 PB0
#define PT100_2 PB1
#define PT100_3 PF11
#define PT100_4 PF12

#define TEMP1 PA4
#define TEMP2 PF10
#define TEMP3 PA3
#define TEMP4 PA0

#define CONTACTOR1 PF2
#define CONTACTOR2 PF1
#define CONTACTOR3 PE6

#define GPIO_1 PD8
#define GPIO_2 PD9
#define GPIO_3 PD10
#define GPIO_4 PD11

namespace BCU::Pinout {

Pin& uart_tx_pin{UART_TX_M_RX_S};
Pin& uart_rx_pin{UART_RX_M_TX_S};

Pin& spi_chip_select_pin{SPI_CS};
Pin& spi_mosi_pin{SPI_MOSI};
Pin& spi_miso_pin{SPI_MISO};
Pin& spi_clock_pin{SPI_CLK};

Pin& can_tx_pin{M_TXD};
Pin& can_rx_pin{M_RXD};
Pin& can_silent_mode_pin{SLNT_Master};

Pin& led_operational_pin{LED_OPERATIONAL_M};
Pin& led_fault_pin{LED_FAULT_M};
Pin& led_can_pin{LED_CAN_M};
Pin& led_flash_pin{LED_FLASH_M};
Pin& led_sleep_pin{LED_SLEEP_M};

Pin& buffer_enable_1_pin{ENABLE_BUFFER_M_1};
Pin& buffer_enable_2_pin{ENABLE_BUFFER_M_2};
Pin& buffer_enable_3_pin{ENABLE_BUFFER_M_3};

Pin& inverter_temp_1_pin{TEMP1};
Pin& inverter_temp_2_pin{TEMP2};
Pin& inverter_temp_3_pin{TEMP3};
Pin& inverter_temp_4_pin{TEMP4};

Pin& booster_temp_1_pin{PT100_1};
Pin& booster_temp_2_pin{PT100_2};
Pin& booster_temp_3_pin{PT100_3};
Pin& booster_temp_4_pin{PT100_4};

Pin& gpio_1_pin{GPIO_1};
Pin& gpio_2_pin{GPIO_2};
Pin& gpio_3_pin{GPIO_3};
Pin& gpio_4_pin{GPIO_4};

};  // namespace BCU::Pinout