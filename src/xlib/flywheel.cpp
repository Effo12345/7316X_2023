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

        currentError = targetVelocity - (getActualVelocity() * 18);
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

    //Overload of moveVelocity function to take an std::pair
    void Flywheel::moveVelocity(std::pair<int, float> vel) {
        moveVelocity(vel.first, vel.second);
    }

    //Based on the TaskWrapper, loop is called by startTask()
    //Running asynchronously, this function computes the desired flywheel velocity
    //and sets the motor base class to the correct voltage
    void Flywheel::loop() {       
        while(true) {
            //If the flywheel should spin backwards, set it to negative voltage
            //and continue
            if(doBackSpin) {
                moveVoltage(-12000);
                continue;
            }

            float measuredVel = getActualVelocity() * 18;
            float filteredVel = velFilter->filter(measuredVel);

            currentError = targetVelocity - filteredVel;

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

            moveVoltage(drive * 12000);

            grapher.newData(targetVelocity, 0);
            grapher.newData(measuredVel, 1);
            grapher.newData(filteredVel, 2);
            grapher.newData(drive * 3600, 3);

            pros::delay(20);
        }
    }

    //Initialize dependencies
    void Flywheel::init() {
        grapher.initGraph({0, 3600}, 250);
        selector.setActive(false);
        startTask();
        active = true;
    }

    //Stop the flywheel (voltage control)
    void Flywheel::stop() {
        moveVoltage(0);
        //fclose(tbhTelem);
    }

    //Run the flywheel in reverse to unjam and for match loads
    void Flywheel::toggleReverse() {
        if(!active)
            init();

        doBackSpin = !doBackSpin;
    }

    std::function<void(int, float)> Flywheel::getAnonymousVelocitySetter() {
        std::function<void(int, float)> f = [=](int vel, float predictedDrive) {
            this->moveVelocity(vel, predictedDrive);
        };
        return f;
    }

    Flywheel::Flywheel(std::int8_t iport, float igain, float ifilterAlpha, Selector& sel)
        : okapi::Motor{iport}, //Calls the constructor for okapi::Motor 
          gain{igain}, selector{sel} {
            velFilter = std::make_shared<okapi::EmaFilter>(ifilterAlpha);
          }
}