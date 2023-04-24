#include "globals.hpp"
#include "autonomous.hpp"
#include "xlib/chassis/extendedchassisbuilder.hpp"
using namespace xlib;

//Controller
Controller master(ControllerId::master);

std::uint8_t portExtender = 15;

//Custom chassis object to wrap drivetrain motors and control movement
std::shared_ptr<ExtendedChassis> chassis = ExtendedChassisBuilder()
    .withMotors({-18, -19, 20}, {11, -12, 13})
    .withDimensions({AbstractMotor::gearset::blue, (4.0 / 5.0)}, {{2.8_in, 12_in}, quadEncoderTPR}, 14.9_in)
    .withMaxVelocity(480)
    .withSensors({{portExtender, 'G', 'H'}, true}, 16)
    .withGains(
        {0.1, 0.005, 0.00165}, // Distance controller gains
        {0.03, 0.000, 0.0012}, // Turn controller gains
        {0.04, 0.0, 0.0},  // Angle controller gains
        {2.5, 0.01, 0.4} //Pure pursuit gains
    )
    .withVelocityConstants(480, 200, 4.0, 200)
    .withLookahead(10_in)
    .build();

//Custom flywheel object to implement TBH control
Flywheel fw(-1, 0.0003f, 0.4f, selector);

//Motor that runs intake, indexer, and roller mechanism
PrimaryMotor primary(-10, {0.017, 0.0, 0.01}, -300_deg, fw.getAnonymousVelocitySetter());

//Pneumatics
Pneumatics lowExpansion1 {{portExtender, 'B'}};
Pneumatics lowExpansion2 {{portExtender, 'C'}};
Pneumatics highExpansion {{portExtender, 'A'}};
Pneumatics angleAdjuster {{portExtender, 'D'}};

//Custom auton selector object. Takes button names and functions to run
Selector selector({
    {{"L 6 Disc", left6Disc}, {"L 3 Disc", left3Disc}, {"L Roller", leftRoller}},
    {{"Full WP", fullWP}},
    {{"R 6 Disc", right6Disc}, {"R 3 Disc", right3Disc}, {"R Roller", rightRoller}}
});