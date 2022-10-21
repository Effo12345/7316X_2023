#pragma once
#include "main.h"
#include "xlib/pneumatics.hpp"

//#include "xlib/grapher.hpp"
//#include "xlib/interface.hpp"

//Constants
const int MAX_VOLTAGE = 12000;

//Controller
extern Controller master;

extern MotorGroup driveL;
extern MotorGroup driveR;
extern MotorGroup flyWheel;

extern Motor intake;
extern Motor roller;

extern Pneumatics indexer;

extern std::shared_ptr<OdomChassisController> chassis;