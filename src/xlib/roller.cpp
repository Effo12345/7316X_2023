#include "xlib/roller.hpp"

/*
 * The intake class is called exclusively during autonomous and so is rather
 * basic. In future, it will be updated to utilize an optical sensor for
 * measuring the state of the intake, rather than relying on the distance the
 * intake mechanism has rotated.
 */

namespace xlib {
    //Run the intake for long enough to flip it into a scored position
    void Roller::flip() {
        intake.moveVelocity(200);
        intake.tarePosition();
        while(intake.getPosition() < 30)
            pros::delay(25);
        intake.moveVelocity(0);
    }

    void Roller::skillsFlip() {
        int time = pros::millis();
        intake.moveVelocity(200);
        intake.tarePosition();
        while(intake.getPosition() < 90) {
            if(pros::millis() - time > 2000)
                break;
            pros::delay(25);
        }
        intake.moveVelocity(0);
    }

    void Roller::skillsFlip2() {
        intake.moveVelocity(200);
        intake.tarePosition();
        while(intake.getPosition() < 50)
            pros::delay(25);
        intake.moveVelocity(0);
    }

    Roller rollerMech;
}

