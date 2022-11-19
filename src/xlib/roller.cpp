#include "xlib/roller.hpp"

/*
 * The Roller class is called exclusively during autonomous and so is rather
 * basic. In future, it will be updated to utilize an optical sensor for
 * measuring the state of the roller, rather than relying on the distance the
 * roller mechanism has rotated.
 */

namespace xlib {
    //Run the roller for long enough to flip it into a scored position
    void Roller::flip() {
        roller.moveVelocity(200);
        roller.tarePosition();
        while(roller.getPosition() < 30)
            pros::delay(25);
        roller.moveVelocity(0);
    }

    void Roller::skillsFlip() {
        roller.moveVelocity(200);
        roller.tarePosition();
        while(roller.getPosition() < 120)
            pros::delay(25);
        roller.moveVelocity(0);
    }

    void Roller::skillsFlip2() {
        roller.moveVelocity(200);
        roller.tarePosition();
        while(roller.getPosition() < 200)
            pros::delay(25);
        roller.moveVelocity(0);
    }

    Roller rollerMech;
}

