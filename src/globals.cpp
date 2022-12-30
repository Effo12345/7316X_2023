#include "globals.hpp"
using namespace xlib;

//Controller
Controller master(ControllerId::master);

//Experimental implementation of two-wheel odometry. Default chassis used by the
//rest of the code to control the drivetrain
std::shared_ptr<OdomChassisController> chassis = ChassisControllerBuilder()
    .withMotors({11, -12, 13}, {-1, 2, -3})
    .withMaxVelocity(600)
    //.withSensors(leftEncoder, rightEncoder)
    .withGains(
        {0.003, 0.001, 0.000022}, // Distance controller gains
        {0.08, 0.0025, 0.002}, // Turn controller gains
        {0.0, 0, 0.0}  // Angle controller gains
    )
    //green gearset, tracking wheel diameter (2.75 in), track (7 in), 
    //and TPR (360)
    .withDimensions({AbstractMotor::gearset::blue, (5.0 / 3.0)}, {
        {4.0_in, 11.875_in}, quadEncoderTPR})
    .withOdometry()
    .buildOdometry();

//Motors
Flywheel fw(4, false, AbstractMotor::gearset::blue, 
    AbstractMotor::encoderUnits::degrees);
Motor everythingElse(14, true, AbstractMotor::gearset::red, 
    AbstractMotor::encoderUnits::degrees);

//Pneumatics
Pneumatics expansion(6, false);

//Sensors
ADIEncoder leftEncoder('A', 'B', false);
ADIEncoder middleEncoder('D', 'E', false);
ADIEncoder rightEncoder('G', 'H', true);

IMU gyro(10);

//OkapiLib PID controller objects. They have been tuned using their (kP, kI, kD) 
//values
std::shared_ptr<IterativePosPIDController> turnPID = std::make_shared<IterativePosPIDController>(0.05, 0.005, 0.00165, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 200_ms));
std::shared_ptr<IterativePosPIDController> movePID = std::make_shared<IterativePosPIDController>(0.07, 0.0, 0.002, 0, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms));
std::shared_ptr<IterativePosPIDController> headingPID = std::make_shared<IterativePosPIDController>(0.04, 0.0, 0.0, 0, TimeUtilFactory::createDefault());