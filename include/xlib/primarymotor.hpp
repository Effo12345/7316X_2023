#pragma once
#include "okapi/api.hpp"
using namespace okapi::literals;

namespace xlib {

    class PrimaryMotor : okapi::Motor {
        using flywheelInterface = std::function<void(int, float)>;

        std::shared_ptr<okapi::IterativePosPIDController::Gains> indexerGains;

        void indexerPID(okapi::QAngle setpoint);

        okapi::QAngle distanceToIndex {};

        flywheelInterface setFlywheelVel {};

        bool forceQuit = false;

    public:
        PrimaryMotor(const std::int8_t iport, const okapi::IterativePosPIDController::Gains& igains, const okapi::QAngle indexDistance, flywheelInterface fw);

        void setNormalizedVelocity(float vel);
        float getNormalizedVelocity();
        void stop();

        void lock();
        void unlock();

        void staggeredIndex(int timesToIndex, okapi::QTime delayPerIndex, std::pair<int, int> flywheelVel = {-1, -1});
        void rollerFlip();
        void rollerFlip2();
        void skillsRollerFlip();
    };

}