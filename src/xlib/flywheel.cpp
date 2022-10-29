#include "xlib/flywheel.hpp"

/*
 * The flywheel class implements the Take Back Half (TBH) velocity control
 * algorithm. It takes in a target velocity and operates asynchronously by
 * inheriting from the TaskWrapper to keep the flywheel's velocity as steady
 * as possible
 */

namespace xlib {
    //Set class variables based on input
    void Flywheel::setVelocity(int velocity, float predicted_drive) {
        targetVelocity = velocity;

        currentError = targetVelocity - flyWheel.getActualVelocity();
        prevError = currentError;

        driveApprox = predicted_drive;

        firstCross = true;
        driveAtZero = 0;
    }

    //Set class variables based on input and calculate predicted_drive as 
    //percentage of maximum velocity (600 rpm)
    void Flywheel::setVelocity(int velocity) {
        targetVelocity = velocity;

        currentError = targetVelocity - flyWheel.getActualVelocity();
        prevError = currentError;

        driveApprox = targetVelocity / 600;

        firstCross = true;
        driveAtZero = 0;
    }

    //Called iteratively, this function computes the desired flywheel velocity
    //and sets the flywheel motor group to that value
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

    //Based on the TaskWrapper, loop is called by startTask()
    void Flywheel::loop() {
        while(true) {
            controlVelocity();
            pros::delay(25);
        }
    }

    //Initialize dependencies
    void Flywheel::init() {
        grapher.initGraph();
        //selector.setActive(false);
        startTask();
        active = true;
    }

    //Trivial accessor for boolean value: active
    bool Flywheel::isActive() {
        return active;
    }

    //Trivial setter for gain. Allows for easier flywheel tuning via user interface
    void Flywheel::setGain(double newGain) {
        gain = newGain;
    }

    //Trivial accessor for double value: gain
    double Flywheel::getGain() {
        return gain;
    }

    Flywheel fw;
}