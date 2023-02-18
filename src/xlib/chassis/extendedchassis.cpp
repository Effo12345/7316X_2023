#include "xlib/chassis/extendedchassis.hpp"
#include "odometry.hpp"
#include "okapi/api/control/iterative/iterativePosPidController.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QLength.hpp"
#include "okapi/impl/device/distanceSensor.hpp"
#include <cstdint>
#include <float.h>

namespace xlib {
    /**
     * Takes in data from the builder and initializes any components necessary
     * for managing chassis movement during driver control and autonomous
     *
     * @param ileft Left drivetrain motors
     * @param iright Right drivetrain motors
     * @param igearset Drivetrain external gear ratio and motor cartridge
     * @param iscales Size of and distance to tracking wheels
     * @param right Right (parallel) tracking wheel encoder
     * @param middle Center (perpendicular) tracking wheel encoder
     * @param leftVelocity Rotation sensor to measure left wheel velocity
     * @param rightVelocity Rotation sensor to measure right wheel velocity
     * @param inertial Inertial sensor to get robot's yaw
     * @param idistanceGains Translational PID controller gains
     * @param iturnGains Rotational PID controller gains
     * @param iangleGains Heading adjustment PID controller gains
     * @param set Pure pursuit tuning constants
     */
    ExtendedChassis::ExtendedChassis (
        const MotorGroup& ileft, const MotorGroup& iright, 
        const AbstractMotor::GearsetRatioPair& igearset, const ChassisScales& iscales,
        const ADIEncoder& right, const ADIEncoder& middle, const RotationSensor leftVelocity, const RotationSensor rightVelocity, const IMU& inertial1, const IMU& inertial2, const std::int8_t distance, 
        const IterativePosPIDController::Gains& idistanceGains, const IterativePosPIDController::Gains& iturnGains, const IterativePosPIDController::Gains& iangleGains, const IterativePosPIDController::Gains& idistanceSensorGains,
        const QPath::Settings set
    ) {
        drive = ChassisControllerBuilder().withMotors(ileft, iright).withDimensions(igearset, iscales).build();
        odometer.setScales(iscales);
        odometer.withSensors(right, middle, inertial1, inertial2, leftVelocity, rightVelocity);
        
        distanceSensor = std::make_shared<DistanceSensor>(distance, std::make_unique<MedianFilter<5>>());
        settings = set;

        turnPID = std::make_shared<IterativePosPIDController>(iturnGains, TimeUtilFactory::withSettledUtilParams(2, 2, 200_ms));
        movePID = std::make_shared<IterativePosPIDController>(idistanceGains, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms));
        headingPID = std::make_shared<IterativePosPIDController>(iangleGains, TimeUtilFactory::createDefault());
        distancePID = std::make_shared<IterativePosPIDController>(idistanceSensorGains, TimeUtilFactory::withSettledUtilParams(0.25, 0.25, 200_ms));
    }

    void ExtendedChassis::startOdom() {
        odometer.startLoop();
    }

    void ExtendedChassis::stopOdom() {
        odometer.stopLoop();
    }

    /**
     * Returns the internal okapi chassis model
     */
    std::shared_ptr<ChassisModel> ExtendedChassis::getModel() {
		return drive->getModel();
	}

    /**
     * Follows a path generated from a set of waypoints using the pure pursuit
     * path following algorithm
     *
     * @param path Input waypoints to be processed and followed
     */
    void ExtendedChassis::followNewPath(QPath path) {
        int timesRun = 0;
		pathGenerator.processWaypoints(path, settings);
		pathFollower.setNewPath(&path, &settings, odometer.getPos());
        
        //Take a mutex to ensure only one function can control the drivetrain
        //motors at a time
        motorThreadSafety.take();
		while(!pathFollower.isSettled()) {
			Odom::Velocity vel = pathFollower.step(odometer.getPos(), odometer.getVel());
			(drive->getModel())->tank(vel.leftVel, vel.rightVel);

            timesRun++;
			pros::delay(25);
		}
        motorThreadSafety.give();
        std::cout << timesRun;
	}


    /**
     * Turns to face a point on the field based on the robot's current position
     * using the dot product angle formula.
     *
     * @param ipoint Point to turn towards
     * @param time Force-quit time limit
     * @param ireversed If the front or back of the robot should face the point
     */
    void ExtendedChassis::turnToPoint(QPoint ipoint, QTime time, bool ireversed) {
        //Find the target delta heading
        Odom::QPos pos = odometer.getRawPos();
        QPoint headingVector {cos(pos.a), sin(pos.a)};
        QPoint adjustedPoint {ipoint.y * -1, ipoint.x};

        QPoint targetVector = adjustedPoint - pos.p;

        //Find angle between vectors using cross product formula
        float targetHeading = acos(headingVector.dotProduct(targetVector) / (headingVector.getMagnitude() * targetVector.getMagnitude()));

        std::string angleOutput = "Angle size: " + std::to_string((targetHeading * radian).convert(degree));
        pros::lcd::set_text(5, angleOutput);

        int side = sgn(((-1 * headingVector.y) * targetVector.x) + (headingVector.x * targetVector.y));
        targetHeading = (pos.a) + (targetHeading * side);

        if(!ireversed)
            targetHeading += okapi::pi;

        std::string targetOutput = "Target heading: " + std::to_string((targetHeading * radian).convert(degree));
        pros::lcd::set_text(6, targetOutput);

        std::string headingOutput = "Side: " + std::to_string(side);
        pros::lcd::set_text(7, headingOutput);
        
        turnToAngle(-1 * targetHeading * radian, time);
    }

    /**
     * Set the initial odometry position for the start of auton
     * 
     * @param ipos Starting position (x, y)
     * @param iheading Starting heading (degrees)
     */
    void ExtendedChassis::setOdomPos(QPoint ipos, QAngle iheading) {
        odometer.setPos(ipos, iheading);
    }

    /**
     * Turn to face angle targetAngle using inertial sensor PID
     *
     * @param targetAngle Angle to turn towards
     * @param time Time limit if the loop doesn't reach the target value
     */
    void ExtendedChassis::turnToAngle(QAngle targetAngle, QTime time) {
        turnPID->reset();
        turnPID->setTarget(0);
        turnPID->setIntegratorReset(true);
        turnPID->setIntegralLimits(0.4 / 0.015, -0.4 / 0.015);
        turnPID->setErrorSumLimits(15, 0);

        auto timer = TimeUtilFactory().createDefault().getTimer();
        timer->placeMark();
        motorThreadSafety.take();

        do {
            (drive->getModel())->arcade(0, turnPID->step(-rescale180(targetAngle.convert(degree)- (odometer.getPos().a * radian).convert(degree))));
            //pros::lcd::set_text(1, std::to_string(odometer.getPos().a));
            //std::cout << odometer.getPos().a;
            pros::delay(10);
        }while(!turnPID->isSettled() && timer->getDtFromMark() < time);

        (drive->getModel())->stop();
        (drive->getModel())->resetSensors();
        motorThreadSafety.give();
    }

    /**
     * Drive a distance using motor-encoder translational PID
     *
     * @param target Distance to drive (typically in inches)
     * @param time Time limit if the loop doesn't reach the target value
     */
    void ExtendedChassis::driveDistance(QLength target, QTime time) {
        movePID->reset();
        headingPID->reset();

        movePID->setTarget(0);
        headingPID->setTarget(odometer.getInternalIMU());

        auto timer = TimeUtilFactory().createDefault().getTimer();
        timer->placeMark();
        motorThreadSafety.take();

        do {
            double distance = tickToIn((static_cast<double>((drive->getModel())->getSensorVals()[0] + (drive->getModel())->getSensorVals()[1]) / 2), drive->getChassisScales(), drive->getGearsetRatioPair());
            double error = target.convert(inch) - distance;
            (drive->getModel())->arcade(movePID->step(-error), headingPID->step(odometer.getInternalIMU()));
            pros::delay(10);
        }while(!movePID->isSettled() && timer->getDtFromMark() < time);

        (drive->getModel())->stop();
        motorThreadSafety.give();
    }

    void ExtendedChassis::driveToDistanceFrom(QLength target, QTime time) {
        movePID->reset();
        headingPID->reset();

        movePID->setTarget(0);
        headingPID->setTarget(odometer.getInternalIMU());

        auto timer = TimeUtilFactory().createDefault().getTimer();
        timer->placeMark();
        motorThreadSafety.take();

        do {
            double distanceTo = (distanceSensor->get() * millimeter).convert(inch);
            pros::lcd::set_text(1, std::to_string(distanceTo));
            double error = target.convert(inch) - distanceTo;
            pros::delay(10);
            (drive->getModel())->arcade(distancePID->step(error), 0);
        } while(!distancePID->isSettled() && timer->getDtFromMark() < time);

        (drive->getModel())->stop();
        motorThreadSafety.give();
    }

}