#include "xlib/flywheel.hpp"

/*
 * The flywheel class implements the Take Back Half (TBH) velocity control
 * algorithm. It takes in a target velocity and operates asynchronously by
 * inheriting from the TaskWrapper to keep the flywheel's velocity as steady
 * as possible
 */

namespace xlib {
    //Set class variables based on input
    void Flywheel::moveVelocity(int velocity, float predicted_drive) {
        if(!active)
            init();

        targetVelocity = velocity;

        currentError = targetVelocity - getActualVelocity();
        prevError = currentError;

        //If the predicted drive is unset, set it driveApprox a decent estimate
        //of the target velocity.
        if(predicted_drive == -1)
            driveApprox = targetVelocity;
        //Otherwise, set the drivaApprox value correctly
        else
            driveApprox = predicted_drive;

        firstCross = true;
        driveAtZero = 0;
    }

    //Called iteratively, this function computes the desired flywheel velocity
    //and sets the flywheel motor group to that value
    void Flywheel::controlVelocity() {
        currentError = targetVelocity - getActualVelocity();

        drive = drive + (currentError * gain);

        drive = std::clamp(drive, 0.0f, 600.0f);

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

        moveVoltage((drive / 600) * 12000);

        grapher.newData(targetVelocity, 0);
        grapher.newData(getActualVelocity(), 1);
        pros::lcd::set_text(0, "Target: " 
            + std::to_string(targetVelocity));
        pros::lcd::set_text(1, "Actual: " 
            + std::to_string(getActualVelocity()));

        /*
        std::string output = std::to_string(currentError) + "\n";
        char outputArray[output.size() + 1];
        strcpy(outputArray, output.c_str());
    	fprintf(tbhTelem, "%s\n", outputArray);
        */
    }

    //Based on the TaskWrapper, loop is called by startTask()
    void Flywheel::loop() {
        while(true) {
            controlVelocity();
            pros::delay(20);
        }
    }

    //Initialize dependencies
    void Flywheel::init() {
        grapher.initGraph();
        selector.setActive(false);
        startTask();
        active = true;
    }

    void Flywheel::stop() {
        stopTask();
        moveVoltage(0);

        //fclose(tbhTelem);
    }

    //Trivial setter for gain. Allows for easier flywheel tuning via user 
    //interface
    void Flywheel::setGain(double newGain) {
        gain = newGain;
    }

    //Trivial accessor for double value: gain
    double Flywheel::getGain() {
        return gain;
    }
}