#include "globals.hpp"
using namespace xlib;

//Controller
Controller master(ControllerId::master);

//Motors
Motor driveFL(1, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBL(2, true, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveFR(13, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor driveBR(4, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);
Motor intake(6, true, AbstractMotor::gearset::red, AbstractMotor::encoderUnits::degrees);
Motor flywheel1(17, true, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor flywheel2(8, false, AbstractMotor::gearset::blue, AbstractMotor::encoderUnits::degrees);
Motor roller(9, false, AbstractMotor::gearset::green, AbstractMotor::encoderUnits::degrees);

//Motor groups
MotorGroup driveL({driveFL, driveBL});
MotorGroup driveR({driveFR, driveBR});
MotorGroup flyWheel({flywheel1, flywheel2});

//Pneumatics
Pneumatics indexer(3, true);

//Sensors
ADIEncoder leftEncoder('A', 'B', false);
ADIEncoder middleEncoder('E', 'F', false);
ADIEncoder rightEncoder('G', 'H', true);

IMU gyro(11);

//OkapiLib PID controller objects. They have been tuned using their (kP, kI, kD) values
IterativePosPIDController turnPID = IterativeControllerFactory::posPID(0.08, 0.0025, 0.002);
IterativePosPIDController drivePID = IterativeControllerFactory::posPID(0.003, 0.0001, 0.000022);
IterativePosPIDController driveShortPID = IterativeControllerFactory::posPID(0.006, 0.0001, 0.000022);

//Experimental implementation of two-wheel odometry. Default chassis used by the
//rest of the code to control the drivetrain
std::shared_ptr<OdomChassisController> chassis = ChassisControllerBuilder()
    .withMotors(driveL, driveR)
    .withMaxVelocity(200)
    .withSensors(leftEncoder, rightEncoder)
    .withGains(
        {0.003, 0.001, 0.000022}, // Distance controller gains
        {0.08, 0.0025, 0.002}, // Turn controller gains
        {0.0, 0, 0.0}  // Angle controller gains (helps drive straight)
    )
    //green gearset, tracking wheel diameter (2.75 in), track (7 in), and TPR (360)
    .withDimensions(AbstractMotor::gearset::green, {{2.75_in, 6.3125_in}, quadEncoderTPR})
    .withOdometry()
    .buildOdometry();