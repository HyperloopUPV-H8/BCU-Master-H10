#ifndef SIM_ON
#include "main.h"

#include "lwip.h"
#endif

#include "BCU/Board.hpp"
#include "ST-LIB.hpp"

bool transmit_can = false;
int main(void) {
#ifdef SIM_ON
    SharedMemory::start();
#endif
    BCU::Board board{};

    while (1) {
        board.update();
    }
}

void Error_Handler(void) {
    ErrorHandler("HAL error handler triggered");
    while (1) {
    }
}
