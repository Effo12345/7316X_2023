#include "globals.hpp"
using namespace xlib;

//Controller
Controller master(ControllerId::master);

//Motors
Motor driveFL(11, false, AbstractMotor::gearset::blue, 
    AbstractMotor::encoderUnits::degrees);
Motor driveBL(12, true, AbstractMotor::gearset::blue, 
    AbstractMotor::encoderUnits::degrees);
Motor driveTL(13, false, AbstractMotor::gearset::blue,
    AbstractMotor::encoderUnits::degrees);
Motor driveFR(1, true, AbstractMotor::gearset::blue, 
    AbstractMotor::encoderUnits::degrees);
Motor driveBR(2, false, AbstractMotor::gearset::blue, 
    AbstractMotor::encoderUnits::degrees);
Motor driveTR(3, true, AbstractMotor::gearset::blue, 
    AbstractMotor::encoderUnits::degrees);
Flywheel fw(4, false, AbstractMotor::gearset::blue, 
    AbstractMotor::encoderUnits::degrees);
Motor everythingElse(14, true, AbstractMotor::gearset::red, 
    AbstractMotor::encoderUnits::degrees);

//Motor groups
MotorGroup driveL({driveFL, driveBL, driveTL});
MotorGroup driveR({driveFR, driveBR, driveTR});

//Pneumatics
Pneumatics expansion(6, false);

//Sensors
ADIEncoder leftEncoder('A', 'B', false);
ADIEncoder middleEncoder('D', 'E', false);
ADIEncoder rightEncoder('G', 'H', true);

IMU gyro(19);

//OkapiLib PID controller objects. They have been tuned using their (kP, kI, kD) 
//values
IterativePosPIDController turnPID = IterativeControllerFactory::posPID (
    0.03, 
    0.01, 
    0.0006
);
IterativePosPIDController drivePID = IterativeControllerFactory::posPID (
    0.004, 
    0.00005, 
    0.0001
);
IterativePosPIDController driveShortPID = IterativeControllerFactory::posPID (
    0.006, 
    0.0001, 
    0.000022
);

//Experimental implementation of two-wheel odometry. Default chassis used by the
//rest of the code to control the drivetrain
std::shared_ptr<OdomChassisController> chassis = ChassisControllerBuilder()
    .withMotors(driveL, driveR)
    .withMaxVelocity(200)
    .withSensors(leftEncoder, rightEncoder)
    .withGains(
        {0.003, 0.001, 0.000022}, // Distance controller gains
        {0.08, 0.0025, 0.002}, // Turn controller gains
        {0.0, 0, 0.0}  // Angle controller gains
    )
    //green gearset, tracking wheel diameter (2.75 in), track (7 in), 
    //and TPR (360)
    .withDimensions(AbstractMotor::gearset::green, {
        {2.75_in, 6.3125_in}, quadEncoderTPR})
    .withOdometry()
    .buildOdometry();