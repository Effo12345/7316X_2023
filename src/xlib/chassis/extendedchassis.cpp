#include "xlib/chassis/extendedchassis.hpp"

namespace xlib {
     /**
      * Takes in data from the builder and initializes any components necessary
      * for managing chassis movement during driver control and autonomous
      * 
      * @param ileft Left drivetrain motors
      * @param iright Right drivetrain motors
      * @param igearset Drivetrain external gear ratio and motor cartridge
      * @param iscales Size of and distance to tracking wheels
      * @param tracking Parallel tracking wheel encoder
      * @param inertial Inertial sensor to get robot's yaw
      * @param idistanceGains Translational PID controller gains (deprecated)
      * @param iturnGains Rotational PID controller gains
      * @param iangleGains Heading adjustment PID controller gains (deprecated)
      * @param set Pure pursuit tuning constants
      */
    ExtendedChassis::ExtendedChassis (
        const MotorGroup& ileft, const MotorGroup& iright, 
        const AbstractMotor::GearsetRatioPair& igearset, const ChassisScales& iscales,
        const ADIEncoder& tracking, const IMU& inertial, 
        const IterativePosPIDController::Gains& idistanceGains, const IterativePosPIDController::Gains& iturnGains, const IterativePosPIDController::Gains& iangleGains,
        const QPath::Settings set
    ) {
        drive = ChassisControllerBuilder().withMotors(ileft, iright).withDimensions(igearset, iscales).build();
        odometer.setScales(iscales.wheelDiameter);
        odometer.withSensors(tracking, inertial);

        leftVelocityGetter = std::make_shared<MotorGroup>(ileft);
        rightVelocityGetter = std::make_shared<MotorGroup>(iright);
    
        settings = set;

        turnPID = std::make_shared<IterativePosPIDController>(iturnGains, TimeUtilFactory::withSettledUtilParams(2, 2, 200_ms));
        movePID = std::make_shared<IterativePosPIDController>(idistanceGains, TimeUtilFactory::withSettledUtilParams(2, 2, 100_ms));
        headingPID = std::make_shared<IterativePosPIDController>(iangleGains, TimeUtilFactory::createDefault());
    }

    /**
     * Start odometer's internal thread
     */
    void ExtendedChassis::startOdom(QPoint ipos, QAngle iheading) {
        odometer.setPos(ipos, iheading);
        odometer.startLoop();
    }

    /**
     * Stop odometer's internal thread
     */
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
		pathGenerator.processWaypoints(path, settings);
		pathFollower.setNewPath(&path, &settings, odometer.getPos());

        MedianFilter<5> leftFilter;
        MedianFilter<5> rightFilter;
        
        //Take a mutex to ensure only one function can control the drivetrain
        //motors at a time
        motorThreadSafety.take();
		do {
            Odom::Velocity chassisVel = {leftVelocityGetter->getActualVelocity(), rightVelocityGetter->getActualVelocity()};
            chassisVel = chassisVel * drive->getGearsetRatioPair().ratio;

            chassisVel.leftVel = leftFilter.filter(chassisVel.leftVel);
            chassisVel.rightVel = rightFilter.filter(chassisVel.rightVel);

			Odom::Velocity vel = pathFollower.step(odometer.getPos(), chassisVel);
			(drive->getModel())->tank(vel.leftVel, vel.rightVel);

			pros::delay(25);
		} while(!pathFollower.isSettled());
        motorThreadSafety.give();
	}


    /**
     * Turns to face a point on the field based on the robot's current position
     * using the dot product angle formula. Not fully tested with one wheel odom
     *
     * @param ipoint Point to turn towards
     * @param time Force-quit time limit
     * @param ireversed If the front or back of the robot should face the point
     */
    void ExtendedChassis::turnToPoint(QPoint ipoint, QTime time, bool ireversed) {
        //Find the target delta heading
        Odom::QPos pos = odometer.getPos();
        QPoint headingVector {cos(pos.a), sin(pos.a)};
        QPoint adjustedPoint {ipoint.y * -1, ipoint.x};

        QPoint targetVector = adjustedPoint - pos.p;

        //Find angle between vectors using cross product formula
        float targetHeading = acos(headingVector.dotProduct(targetVector) / (headingVector.getMagnitude() * targetVector.getMagnitude()));


        int side = sgn(((-1 * headingVector.y) * targetVector.x) + (headingVector.x * targetVector.y));
        targetHeading = (pos.a) + (targetHeading * side);

        if(ireversed)
            targetHeading += okapi::pi;
        
        turnToAngle(targetHeading * radian, time);
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

        auto timer = TimeUtilFactory().createDefault().getTimer();
        timer->placeMark();
        motorThreadSafety.take();

        do {
            (drive->getModel())->arcade(0, std::clamp(turnPID->step(-rescale180(targetAngle.convert(degree)- odometer.getPos().a)), -PIDvelocityLimit, PIDvelocityLimit));
            pros::delay(10);
        }while(!turnPID->isSettled() && timer->getDtFromMark() < time);

        (drive->getModel())->stop();
        (drive->getModel())->resetSensors();
        motorThreadSafety.give();
    }

    /**
     * Drive a distance using parallel tracking wheel translational PID
     *
     * @param target Distance to drive (typically in inches)
     * @param time Time limit if the loop doesn't reach the target value
     */
     //Thought I'd leave this here for anyone that wants it.
     //NOT FUNCTIONAL in the current build
    /*
    void ExtendedChassis::driveDistance(QLength target, QTime time) {
        movePID->reset();
        headingPID->reset();

        movePID->setTarget(0);
        headingPID->setTarget(odometer.getInternalIMU());

        auto timer = TimeUtilFactory().createDefault().getTimer();
        timer->placeMark();
        motorThreadSafety.take();

        do {
            double distance = (odometer.getTrack() - relativeTrackingDistance).convert(inch);
            double error = target.convert(inch) - distance;
            (drive->getModel())->arcade(std::clamp(movePID->step(-error), -PIDvelocityLimit, PIDvelocityLimit), headingPID->step(odometer.getInternalIMU()));
            pros::delay(10);
        }while(!movePID->isSettled() && timer->getDtFromMark() < time);

        (drive->getModel())->stop();
        motorThreadSafety.give();
    }
    */

    void ExtendedChassis::setPIDVelocityLimit(double velLimit) {
        PIDvelocityLimit = std::clamp(velLimit, 0.0, 1.0);
    }

}