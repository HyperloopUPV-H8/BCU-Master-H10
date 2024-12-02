#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "BCU/BCU.hpp"

using enum CMS::DataRequestFlags;
bool transmit_can = false;
int main(void) {
    BCU::BCU bcu = BCU::BCU();
    BCU::Communication::FDCAN::request_all_data(EnableAutoTx | SendCellsVoltage,1000);

    while (1) {

        bcu.update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
