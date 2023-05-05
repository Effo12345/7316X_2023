#pragma once
#include "xlib/api.hpp"
#include "autonomous.hpp"

//Constants
const int MAX_VOLTAGE = 12000;

//Controller
extern Controller master;

extern Flywheel fw;
extern PrimaryMotor primary;

extern Pneumatics lowExpansion1;
extern Pneumatics lowExpansion2;
extern Pneumatics highExpansion;
extern Pneumatics angleAdjuster;

extern ADIEncoder rightEncoder;

extern std::shared_ptr<xlib::ExtendedChassis> chassis;

extern Selector selector;