#include "primarymotor.hpp"

namespace xlib {
    /**
     * @param vel Velocity on [-1, 1]
     */
    void PrimaryMotor::setNormalizedVelocity(float vel) {
        moveVoltage(std::clamp(vel, -1.0f, 1.0f) * 12000);
    }

    /**
     * @return Velocity on [-1, 1]
     */
    float PrimaryMotor::getNormalizedVelocity() {
        return std::clamp(static_cast<float>(getActualVelocity() / 200), -1.0f, 1.0f);
    }

    //Set velocity to 0
    void PrimaryMotor::stop() {
        setNormalizedVelocity(0.0);
    }


    //Lock/unlock the primary motor to be used as a passive roller mech
    void PrimaryMotor::lock() {
        setBrakeMode(AbstractMotor::brakeMode::brake);
    }

    void PrimaryMotor::unlock() {
        setBrakeMode(AbstractMotor::brakeMode::coast);
    }

    /**
     * Run the primary motor to a target rotational positin using PID
     * 
     * @param setpoint Desired rotation angle (deg)
     */
    void PrimaryMotor::indexerPID(okapi::QAngle setpoint) {
        float error;
        float prevError;
        float pPower;
        float iPower;
        float dPower;
        float pwr;

        okapi::Timer time;
        time.placeMark();

        do {
            pPower = error = setpoint.convert(okapi::degree) - getPosition();

            iPower += error; 

            dPower = error - prevError;
            prevError = error;

            pwr = (pPower * indexerGains->kP) + (iPower * indexerGains->kI) + (dPower * indexerGains->kD);
        
            setNormalizedVelocity(std::clamp(pwr, -1.0f, 1.0f));
            pros::lcd::set_text(1, std::to_string(getPosition()));

            //Time limit index to prevent jamming and ensure roller scoring
            if(time.getDtFromMark() < 500_ms) {
                forceQuit = true;
                break;
            }

            pros::delay(20);
        } while(std::fabs(error) > 10);

        stop();
    }

    /**
     * Call indexerPID a pre-determined number of times with a set delay between
     * each call.
     * 
     * @param timesToIndex Maximum number of index attempts
     * @param delayPerIndex Time between the start of each PID rotation
     */
    void PrimaryMotor::staggeredIndex(int timesToIndex, okapi::QTime delayPerIndex) {
        for(int i = 0; i < timesToIndex; i++) {
            tarePosition();
            indexerPID(distanceToIndex);

            //Stop indexing and continue if the flywheel gets jammed
            if(forceQuit) {
                break;
                forceQuit = false;
            }

            pros::delay(delayPerIndex.convert(okapi::millisecond));
        }
    }


    void PrimaryMotor::rollerFlip() {
        okapi::Timer time;
        time.placeMark();
        tarePosition();
        setNormalizedVelocity(-1);
        while(getPosition() > -500 && time.getDtFromMark() < 500_ms) 
            pros::delay(25);
        stop();
    }

    void PrimaryMotor::rollerFlip2() {
        okapi::Timer time;
        time.placeMark();
        tarePosition();
        setNormalizedVelocity(-1);
        while(getPosition() > -300 && time.getDtFromMark() < 500_ms) 
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

    PrimaryMotor::PrimaryMotor(const std::int8_t iport, const okapi::IterativePosPIDController::Gains& igains, const okapi::QAngle indexDistance)
        : okapi::Motor{iport}, //Calls the constructor for okapi::Motor 
          distanceToIndex{indexDistance}
          {
            indexerGains = std::make_shared<okapi::IterativePosPIDController::Gains>(igains);
          }
}