#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "BCU/BCU.hpp"

int main(void) {
    BCU::BCU bcu = BCU::BCU();

    while (1) {
        bcu.update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
