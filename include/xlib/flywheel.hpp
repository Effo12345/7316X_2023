#pragma once
#include "okapi/impl/device/motor/motor.hpp"
#include "taskwrapper.hpp"
#include "grapher.hpp"
#include "utils.hpp"
#include "selector.hpp"

namespace xlib {
    class Flywheel : TaskWrapper, okapi::Motor {
        float targetVelocity = 0.0f;
        float driveApprox = 0.0f;
        bool firstCross = false;
        float driveAtZero = 0.0f;
        float currentError;
        float prevError;
        float drive = 0.0f;

        bool active = false;

        Grapher grapher;
        
        //FILE* targetVelocityTelem = fopen("/usd/TBH/targetVelocity.txt", "w");
        //FILE* measuredVelocityTelem = fopen("/usd/TBH/measuredVelocity.txt", "w");
        //FILE* deltaVelocity= fopen("/usd/deltaVel.txt", "w");

        void loop();

        void init();
        void stop();

    public:

        using Motor::Motor;

        void moveVelocity(int velocity, float predicted_drive = -1);
    };
}


