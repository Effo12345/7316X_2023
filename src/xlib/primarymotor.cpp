#include "primarymotor.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QTime.hpp"

namespace xlib {
    void PrimaryMotor::setNormalizedVelocity(float vel) {
        moveVoltage(std::clamp(vel, -1.0f, 1.0f) * 12000);
    }

    float PrimaryMotor::getNormalizedVelocity() {
        return std::clamp(static_cast<float>(getActualVelocity() / 200), -1.0f, 1.0f);
    }

    void PrimaryMotor::stop() {
        setNormalizedVelocity(0.0);
    }

    void PrimaryMotor::indexerPID(okapi::QAngle setpoint) {
        float error;
        float prevError;
        float pPower;
        float iPower;
        float dPower;
        float pwr;

        do {
            pPower = error = setpoint.convert(okapi::degree) - getPosition();

            iPower += error; 

            dPower = error - prevError;
            prevError = error;

            pwr = (pPower * indexerGains->kP) + (iPower * indexerGains->kI) + (dPower * indexerGains->kD);
        
            setNormalizedVelocity(std::clamp(pwr, -1.0f, 1.0f));
            pros::lcd::set_text(1, std::to_string(getPosition()));

            pros::delay(20);
        } while(std::fabs(error) > 10);

        stop();
    }

    void PrimaryMotor::debugPID(okapi::QAngle setpoint) {
        indexerPID(setpoint);
    }

    void PrimaryMotor::staggeredIndex(int timesToIndex, okapi::QTime delayPerIndex) {
        for(int i = 0; i < timesToIndex; i++) {
            tarePosition();
            indexerPID(-300_deg);
            pros::delay(delayPerIndex.convert(okapi::millisecond));
        }
    }

    void PrimaryMotor::rollerFlip() {
        tarePosition();
        setNormalizedVelocity(1);
        while(getPosition() < 80) 
            pros::delay(25);
        stop();
    }

    void PrimaryMotor::skillsRollerFlip() {
        tarePosition();
        setNormalizedVelocity(1);
        while(getPosition() < 170)
            pros::delay(25);
        stop();
    }

    PrimaryMotor::PrimaryMotor(const std::int8_t iport, const okapi::IterativePosPIDController::Gains& igains)
        : okapi::Motor{iport} //Calls the constructor for okapi::Motor 
          {
            indexerGains = std::make_shared<okapi::IterativePosPIDController::Gains>(igains);
          }
}