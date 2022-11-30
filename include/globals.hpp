#pragma once
#include "main.h"
#include "xlib/pneumatics.hpp"
#include "xlib/flywheel.hpp"

//#include "xlib/grapher.hpp"
//#include "xlib/interface.hpp"

//Constants
const int MAX_VOLTAGE = 12000;

//Controller
extern Controller master;

extern MotorGroup driveL;
extern MotorGroup driveR;

extern Flywheel fw;
extern Motor intake;

extern Pneumatics indexer;
extern Pneumatics expansion;

extern ADIEncoder rightEncoder;
extern IMU gyro;

extern IterativePosPIDController turnPID;
extern IterativePosPIDController drivePID;
extern IterativePosPIDController driveShortPID;
extern std::shared_ptr<OdomChassisController> chassis;