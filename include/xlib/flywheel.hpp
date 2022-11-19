#pragma once
#include "xlib/grapher.hpp"
#include "globals.hpp"

namespace xlib {
    class Flywheel : TaskWrapper {
        float targetVelocity = 0.0f;
        float driveApprox = 0.0f;
        bool firstCross = false;
        float driveAtZero = 0.0f;
        float currentError;
        float prevError;
        float drive = 0.0f;

        double gain = 0.027f;

        bool active = false;
        
        //FILE* targetVelocityTelem = fopen("/usd/TBH/targetVelocity.txt", "w");
        //FILE* measuredVelocityTelem = fopen("/usd/TBH/measuredVelocity.txt", "w");
        //FILE* tbhTelem = fopen("/usd/tbh.txt", "w");

        void loop();

    public:

        Flywheel() = default;
        ~Flywheel() = default;

        void setVelocity(int velocity, float predicted_drive);

        void test();

        void setVelocity(int velocity);

        void init();
        void stop();

        bool isActive();

        void setGain(double newGain);

        double getGain();

        void controlVelocity();
    };

    extern Flywheel fw;
}


