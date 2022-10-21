#include "xlib/pneumatics.hpp"

namespace xlib {
    Pneumatics::Pneumatics(char iPort, bool initState):piston(iPort), state(initState){
        piston.set_value(state);
    }

    void Pneumatics::toggle(){
        state = !state;
        piston.set_value(state);
    }

    void Pneumatics::set(bool iState){
        state = iState;
        piston.set_value(iState);
    }

    void Pneumatics::index() {
        startTask();
    }

    void Pneumatics::loop() {
        toggle();
        pros::delay(100);
        toggle();
        stopTask();
    }

    bool Pneumatics::getState() const{
        return state;
    }
}