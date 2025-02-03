#pragma once

#include "CMS-LIB.hpp"
#include "ST-LIB.hpp"

namespace BCU::Communication::Orders {
class RestartSupercaps {
    inline static Order* order{nullptr};
    inline static bool received{false};

    static void callback() { received = true; }

    static void execute();

   public:
    constexpr static uint16_t id{900};

    RestartSupercaps() {
        if (order == nullptr) order = new StackOrder(id, callback);
    }

    static void update() {
        if (received) {
            execute();
            received = false;
        }
    }
};

class OpenContactors {
    inline static Order* order{nullptr};
    inline static bool received{false};

    static void callback() { received = true; }

    static void execute();

   public:
    constexpr static uint16_t id{901};

    OpenContactors() {
        if (order == nullptr) order = new StackOrder(id, callback);
    }

    static void update() {
        if (received) {
            execute();
            received = false;
        }
    }
};

class CloseContactors {
    inline static Order* order{nullptr};
    inline static bool received{false};

    static void callback() { received = true; }

    static void execute();

   public:
    constexpr static uint16_t id{902};

    CloseContactors() {
        if (order == nullptr) order = new StackOrder(id, callback);
    }

    static void update() {
        if (received) {
            execute();
            received = false;
        }
    }
};

};  // namespace BCU::Communication::Orders