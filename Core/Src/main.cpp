#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "BCU/BCU.hpp"

bool transmit_can = false;
int main(void) {
#ifdef SIM_ON
    SharedMemory::start();
#endif

    BCU::Board bcu;

    bcu.start();

    while (1) {
        bcu.update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
