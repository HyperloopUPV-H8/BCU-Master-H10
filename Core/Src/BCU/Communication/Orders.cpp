#include "BCU/Communication/Orders.hpp"

#include "BCU/BCU.hpp"

using namespace BCU;

void Communication::Orders::RestartSupercaps::execute() { FDCAN::restart(); }

void Communication::Orders::OpenContactors::execute() {
    Board::contactors.open();
}

void Communication::Orders::CloseContactors::execute() {
    Board::contactors.close();
}