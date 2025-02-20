#include "BCU/Communication/Orders.hpp"

#include "BCU/BCU.hpp"

using namespace BCU;

void Communication::Orders::RestartSupercaps::execute() { FDCAN::restart(); }

void Communication::Orders::OpenContactors::execute() {
    if (Board::instance == nullptr) {
        ErrorHandler("Board instance not initialized");
        return;
    }
    Board::instance->contactors.open();
}

void Communication::Orders::CloseContactors::execute() {
    if (Board::instance == nullptr) {
        ErrorHandler("Board instance not initialized");
        return;
    }
    Board::instance->contactors.close();
}