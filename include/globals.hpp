#pragma once
#include "main.h"
#include "okapi/api/control/iterative/iterativePosPidController.hpp"
#include "xlib/pneumatics.hpp"
#include "xlib/flywheel.hpp"
#include "xlib/selector.hpp"
#include "autonomous.hpp"

//Constants
const int MAX_VOLTAGE = 12000;

//Controller
extern Controller master;

extern Flywheel fw;
extern Motor everythingElse;

extern Pneumatics indexer;
extern Pneumatics expansion;

extern ADIEncoder rightEncoder;
extern IMU gyro;

extern std::shared_ptr<IterativePosPIDController> turnPID;
extern std::shared_ptr<IterativePosPIDController> movePID;
extern std::shared_ptr<IterativePosPIDController> headingPID;
extern std::shared_ptr<OdomChassisController> chassis;

extern Selector selector;