#pragma once
#include "main.h"
#include "xlib/pneumatics.hpp"
#include "xlib/flywheel.hpp"
#include "xlib/display/selector.hpp"
#include "xlib/chassis/extendedchassisbuilder.hpp"
#include "autonomous.hpp"

//Constants
const int MAX_VOLTAGE = 12000;

//Controller
extern Controller master;

extern Flywheel fw;
extern Motor everythingElse;

extern Pneumatics expansion;
extern Pneumatics angleAdjuster;

extern ADIEncoder rightEncoder;

extern std::shared_ptr<xlib::ExtendedChassis> chassis;

extern Selector selector;