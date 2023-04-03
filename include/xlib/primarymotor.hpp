#pragma once
#include "okapi/api.hpp"
#include "okapi/api/control/iterative/iterativePosPidController.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QTime.hpp"

using namespace okapi::literals;

namespace xlib {

    class PrimaryMotor : okapi::Motor {
        std::shared_ptr<okapi::IterativePosPIDController::Gains> indexerGains;

        void indexerPID(okapi::QAngle setpoint);
    public:
        PrimaryMotor(const std::int8_t iport, const okapi::IterativePosPIDController::Gains& igains);

        void setNormalizedVelocity(float vel);
        float getNormalizedVelocity();
        void stop();

        void staggeredIndex(int timesToIndex, okapi::QTime delayPerIndex);
        void rollerFlip();
        void skillsRollerFlip();

        void debugPID(okapi::QAngle setpoint);
    };

}