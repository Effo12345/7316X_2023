#include "xlib/pneumatics.hpp"

/*
 * The Pneumatics class wraps VEX's built-in pneumatics implementation 
 * (pros::ADIDigitalOut) and gives it additional functionality, such as persistent
 * state and the ability to update its state asynchronously
 */

namespace xlib {
    //Constructor to initialize port and default state
    Pneumatics::Pneumatics(char iPort, bool initState):piston(iPort), state(initState){
        piston.set_value(state);
    }

    //Set state to its opposite
    void Pneumatics::toggle(){
        state = !state;
        piston.set_value(state);
    }

    //Set state of the piston to iState
    void Pneumatics::set(bool iState){
        state = iState;
        piston.set_value(iState);
    }

    //Actuate the piston, delay 150 msec, retract the piston. Used in the
    //flywheel's indexer
    void Pneumatics::index() {
        startTask();
    }

    //Inherited from the TaskWrapper class, loop is called by startTask() and
    //runs asynchronously. loop() implements the functionality attributed to index()
    void Pneumatics::loop() {
        toggle();
        pros::delay(150);
        toggle();
        stopTask();
    }

    //Trivial (const) getter for boolean: state
    bool Pneumatics::getState() const{
        return state;
    }
}