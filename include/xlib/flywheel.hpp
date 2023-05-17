#pragma once
#include "okapi/api/filter/emaFilter.hpp"
#include "okapi/impl/device/motor/motor.hpp"
#include "taskwrapper.hpp"
#include "xlib/display/grapher.hpp"
#include "utils.hpp"
#include "xlib/display/selector.hpp"

namespace xlib {
    class Flywheel : TaskWrapper, okapi::Motor {
        float targetVelocity = 0.0f;
        float driveApprox = 0.0f;
        bool firstCross = false;
        float driveAtZero = 0.0f;
        float currentError;
        float prevError;
        float drive = 0.0f;

        double gain = 0.0f;

        std::shared_ptr<okapi::EmaFilter> velFilter;

        bool active = false;
        bool doBackSpin = false;

        Grapher grapher;
        Selector& selector;

        void loop();

        void init();

    public:

        Flywheel(std::int8_t iport, float igain, float ifilterAlpha, Selector& sel);

        void moveVelocity(int velocity, float predicted_drive = -1);
        void moveVelocity(std::pair<int, float> vel);
        void toggleReverse();
        void stop();
    };
}


