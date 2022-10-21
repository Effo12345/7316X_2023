#pragma once
#include <iostream>
#include <string>
#include "flywheel.hpp"
#include "grapher.hpp"

namespace xlib {
    class Interface {
        std::string inputs[2] {"update_gain", "clear_graph"};
        public:
        void update();
    };
    extern Interface interface;
}