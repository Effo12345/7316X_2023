#include "xlib/roller.hpp"

/*
 * The everythingElse class is called exclusively during autonomous and so is rather
 * basic. In future, it will be updated to utilize an optical sensor for
 * measuring the state of the everythingElse, rather than relying on the distance the
 * everythingElse mechanism has rotated.
 */

namespace xlib {
    //Run the everythingElse for long enough to flip it into a scored position
    void Roller::flip() {
        everythingElse.moveVelocity(200);
        everythingElse.tarePosition();
        while(everythingElse.getPosition() < 30)
            pros::delay(25);
        everythingElse.moveVelocity(0);
    }

    void Roller::skillsFlip() {
        int time = pros::millis();
        everythingElse.moveVelocity(200);
        everythingElse.tarePosition();
        while(everythingElse.getPosition() < 90) {
            if(pros::millis() - time > 2000)
                break;
            pros::delay(25);
        }
        everythingElse.moveVelocity(0);
    }

    void Roller::skillsFlip2() {
        everythingElse.moveVelocity(200);
        everythingElse.tarePosition();
        while(everythingElse.getPosition() < 50)
            pros::delay(25);
        everythingElse.moveVelocity(0);
    }

    Roller rollerMech;
}

