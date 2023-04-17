#pragma once
#include "pros/adi.hpp"
#include "xlib/taskwrapper.hpp"

namespace xlib {
    class Pneumatics : TaskWrapper {
        pros::ADIDigitalOut piston;
        bool state;

        public:
        Pneumatics(std::uint8_t iPort, bool initState = false);
        Pneumatics(std::pair<std::uint8_t, std::uint8_t> iPort, bool initState = false);

        Pneumatics(const Pneumatics& iPiston) = delete;

        ~Pneumatics() = default;

        void toggle();

        void set(bool iState);

        void quickAction();

        void loop();

        bool getState() const;
    };
}