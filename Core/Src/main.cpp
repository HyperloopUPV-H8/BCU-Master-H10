#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "ST-LIB.hpp"

bool transmit_can = false;
int main(void) {
#ifdef SIM_ON
    SharedMemory::start();
#endif
    STLIB::start();

    while (1) {
        STLIB::update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
