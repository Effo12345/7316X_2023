#pragma once
#include "pros/adi.hpp"
#include "xlib/taskwrapper.hpp"

namespace xlib {
    class Pneumatics {
        pros::ADIDigitalOut piston;
        bool state;

        public:
        Pneumatics(char iPort, bool initState = false);

        ~Pneumatics() = default;

        void toggle();

        void set(bool iState);

        bool getState() const;
    };
}