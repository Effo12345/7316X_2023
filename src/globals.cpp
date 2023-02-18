#include "globals.hpp"
#include "autonomous.hpp"
#include "xlib/chassis/extendedchassisbuilder.hpp"
using namespace xlib;

//Controller
Controller master(ControllerId::master);

//Custom chassis object to wrap drivetrain motors and control movement
std::shared_ptr<ExtendedChassis> chassis = ExtendedChassisBuilder()
    .withMotors({12, -13, 14}, {-8, 9, -10})
    .withDimensions({AbstractMotor::gearset::blue, (5.0 / 3.0)}, {{2.7936_in, 2.96875_in, 6.34375_in, 2.7936_in}, quadEncoderTPR}, 13.5_in)
    .withMaxVelocity(380)
    .withSensors({'A', 'B'}, {'C', 'D'}, 16, {17, true}, 20, 18, 11)
    .withGains(
        {0.1, 0.005, 0.00165}, // Distance controller gains
        {0.06, 0.0001, 0.002}, // Turn controller gains
        {0.04, 0.0, 0.0},  // Angle controller gains
        {0.06, 0.0, 0.0005}, //Distance sensor gains
        {3.8, 0.01, 0.4} //Pure pursuit gains
    )
    .withVelocityConstants(50, 100, 2.5, 200)
    .withLookahead(15_in)
    .build();

//Custom flywheel object to implement TBH control
Flywheel fw(1, 0.00028f, selector);

//Motor that runs intake, indexer, and roller mechanism
Motor everythingElse(15, true, AbstractMotor::gearset::red, 
    AbstractMotor::encoderUnits::degrees);

//Pneumatics
Pneumatics expansion(6, false);
Pneumatics angleAdjuster(7, false);

//Custom auton selector object. Takes button names and functions to run
Selector selector({
    {{"Left roller", WPL}, {"Left full", FullL}},
    {{"Skills", Skills}},
    {{"Right roller", WPR}}
});