#include "globals.hpp"
using namespace xlib;

//Controllers
Controller master(ControllerId::master);

//Motors
Motor driveFL(1, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBL(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveFR(3, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBR(4, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor intake(6, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor flywheel1(7, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor flywheel2(8, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor roller(9, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

MotorGroup driveL({driveFL, driveBL});
MotorGroup driveR({driveFR, driveBR});
MotorGroup flyWheel({flywheel1, flywheel2});

Pneumatics indexer(3, true);

ADIEncoder leftEncoder('A', 'B', false);
ADIEncoder middleEncoder('E', 'F', false);
ADIEncoder rightEncoder('G', 'H', true);

//Three wheel odom
/*
std::shared_ptr<OdomChassisController> chassis = ChassisControllerBuilder()
    .withMotors(driveL, driveR)
    .withMaxVelocity(200)
    .withSensors(leftEncoder, rightEncoder, middleEncoder)
    .withGains(
        {0.1, 0, 0.1}, // Distance controller gains
        {0.1, 0, 0.1}, // Turn controller gains
        {0.1, 0, 0.1}  // Angle controller gains (helps drive straight)
    )
    // green gearset, tracking wheel diameter (2.75 in), track width (6.3125 in), and TPR (360)
    // 2.25 inch middle encoder distance, and 2.75 inch middle wheel diameter
    .withDimensions(AbstractMotor::gearset::green, {{2.75_in, 6.3125_in, 2.25_in, 2.75_in}, quadEncoderTPR})
    .withOdometry()
    .buildOdometry();
*/

//Two wheel odom
std::shared_ptr<OdomChassisController> chassis = ChassisControllerBuilder()
    .withMotors(driveL, driveR)
    .withMaxVelocity(200)
    .withSensors(leftEncoder, rightEncoder)
    .withGains(
        {0.1, 0, 0.1}, // Distance controller gains
        {0.1, 0, 0.1}, // Turn controller gains
        {0.1, 0, 0.1}  // Angle controller gains (helps drive straight)
    )
    //green gearset, tracking wheel diameter (2.75 in), track (7 in), and TPR (360)
    .withDimensions(AbstractMotor::gearset::green, {{2.75_in, 6.3125_in}, quadEncoderTPR})
    .withOdometry()
    .buildOdometry();