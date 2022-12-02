#include "xlib/pneumatics.hpp"

/*
 * The Pneumatics class wraps VEX's built-in pneumatics implementation 
 * (pros::ADIDigitalOut) and gives it additional functionality, such as persistent
 * state
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

    //Trivial (const) getter for boolean: state
    bool Pneumatics::getState() const{
        return state;
    }
}