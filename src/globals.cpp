#include "globals.hpp"
#include "autonomous.hpp"
#include "xlib/chassis/extendedchassisbuilder.hpp"
using namespace xlib;

//Controller
Controller master(ControllerId::master);

//Custom chassis object to wrap drivetrain motors and control movement
std::shared_ptr<ExtendedChassis> chassis = ExtendedChassisBuilder()
    .withMotors({-12, -13, 14}, {8, 9, -10})
    .withDimensions({AbstractMotor::gearset::blue, (5.0 / 3.0)}, {{2.7936_in}, quadEncoderTPR}, 14.9_in)
    .withMaxVelocity(480)
    .withSensors({'A', 'B'}, 17)
    .withGains(
        {0.1, 0.005, 0.00165}, // Distance controller gains
        {0.06, 0.0001, 0.002}, // Turn controller gains
        {0.04, 0.0, 0.0},  // Angle controller gains
        {3.8, 0.01, 0.4} //Pure pursuit gains
    )
    .withVelocityConstants(360, 50, 4.0, 100)
    .withLookahead(10_in)
    .build();

//Custom flywheel object to implement TBH control
Flywheel fw(-1, 0.00015f, 0.2f, selector);

//Motor that runs intake, indexer, and roller mechanism
PrimaryMotor primary(-15, {0.01, 0.0, 0.02});

//Pneumatics
Pneumatics expansion(6, false);
Pneumatics angleAdjuster(7, false);

//Custom auton selector object. Takes button names and functions to run
Selector selector({
    {{"Left roller", WPL}, {"Left full", FullL}, {"Full WP", FullWP}},
    {{"Skills", Skills}, {"Safety", SafetySkills}},
    {{"Right roller", WPR}, {"Right Full", FullR}}
});