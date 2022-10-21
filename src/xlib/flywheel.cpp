#include "flywheel.hpp"

namespace xlib {
    void Flywheel::setVelocity(int velocity, float predicted_drive) {
        targetVelocity = velocity;

        currentError = targetVelocity - flyWheel.getActualVelocity();
        prevError = currentError;

        driveApprox = predicted_drive;

        firstCross = true;
        driveAtZero = 0;
    }

    void Flywheel::setVelocity(int velocity) {
        targetVelocity = velocity;

        currentError = targetVelocity - flyWheel.getActualVelocity();
        prevError = currentError;

        driveApprox = targetVelocity / 600;

        firstCross = true;
        driveAtZero = 0;
    }

    void Flywheel::controlVelocity() {
        currentError = targetVelocity - flyWheel.getActualVelocity();

        drive = drive + (currentError * gain);

        drive = std::clamp(drive, 0.0f, 1.0f);

        if(sgn(currentError) != sgn(prevError)) {
            if(firstCross) {
                drive = driveApprox;
                firstCross = false;
            }
            else
                drive = 0.5 * (drive + driveAtZero);

            driveAtZero = drive;
        }
        prevError = currentError;

        flyWheel.moveVoltage(drive * MAX_VOLTAGE);

        grapher.newData(targetVelocity, 0);
        grapher.newData(flyWheel.getActualVelocity(), 1);
        pros::lcd::set_text(0, "Target: " + std::to_string(targetVelocity));
        pros::lcd::set_text(1, "Actual: " + std::to_string(flyWheel.getActualVelocity()));
    }

    void Flywheel::setGain(double newGain) {
        gain = newGain;
        return;
    }

    double Flywheel::getGain() {
        return gain;
    }

    Flywheel fw;
}