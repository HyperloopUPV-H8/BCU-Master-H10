#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "BCU/BCU.hpp"

using enum CMS::DataRequestFlags;
bool transmit_can = false;
int main(void) {
    BCU::BCU bcu = BCU::BCU();
    //BCU::Communication::FDCAN::request_all_data(EnableAutoTx | SendCellsVoltage,1000);
    char data[8] = {
    0x55,0x55,0x55,0x55,
    0x55,0x55,0x55,0x55
    };
    Time::register_low_precision_alarm(1000,[&](){
        transmit_can = true;
    });
    while (1) {
        if(transmit_can){
            transmit_can = false;
            FDCAN::transmit(0,85,&data[0],FDCAN::BYTES_8);
        }
        bcu.update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
