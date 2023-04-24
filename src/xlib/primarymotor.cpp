#include "primarymotor.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QTime.hpp"
#include "okapi/impl/util/timer.hpp"

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

    void PrimaryMotor::staggeredIndex(int timesToIndex, okapi::QTime delayPerIndex, std::pair<int, int> flywheelVel) {
        for(int i = 0; i < timesToIndex; i++) {
            tarePosition();
            indexerPID(distanceToIndex);

            if(flywheelVel.first != -1 && i == 1) {
                flywheelVel.first -= flywheelVel.second;
                setFlywheelVel(flywheelVel.first, -1);
            }

            pros::delay(delayPerIndex.convert(okapi::millisecond));
        }
    }

    void PrimaryMotor::rollerFlip() {
        okapi::Timer time;
        time.placeMark();
        tarePosition();
        setNormalizedVelocity(-1);
        while(getPosition() > -400 && time.getDtFromMark() < 500_ms) 
            pros::delay(25);
        stop();
    }

    void PrimaryMotor::rollerFlip2() {
        okapi::Timer time;
        time.placeMark();
        tarePosition();
        setNormalizedVelocity(-1);
        while(getPosition() > -200 && time.getDtFromMark() < 500_ms) 
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

    PrimaryMotor::PrimaryMotor(const std::int8_t iport, const okapi::IterativePosPIDController::Gains& igains, const okapi::QAngle indexDistance, flywheelInterface fw)
        : okapi::Motor{iport}, //Calls the constructor for okapi::Motor 
          distanceToIndex{indexDistance}, setFlywheelVel{fw}
          {
            indexerGains = std::make_shared<okapi::IterativePosPIDController::Gains>(igains);
          }
}