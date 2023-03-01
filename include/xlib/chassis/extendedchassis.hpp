#pragma once
//#include "main.h"
#include "okapi/api/control/iterative/iterativePosPidController.hpp"
#include "okapi/api/units/QLength.hpp"
#include "xlib/chassis/odometry.hpp"
#include "xlib/chassis/pathgenerator.hpp"
#include "xlib/chassis/pathfollower.hpp"
#include "pros/screen.hpp"

namespace xlib {

	class ExtendedChassis {
		std::shared_ptr<ChassisController> drive;
		Odom odometer;
		PathGenerator pathGenerator;
		PathFollower pathFollower;

		std::shared_ptr<DistanceSensor> distanceSensor;

		std::shared_ptr<IterativePosPIDController> turnPID;
		std::shared_ptr<IterativePosPIDController> movePID;
		std::shared_ptr<IterativePosPIDController> headingPID;
		std::shared_ptr<IterativePosPIDController> distancePID;

		IterativePosPIDController::Gains discGrabGains{0.5, 0.0, 0.007};
		std::shared_ptr<IterativePosPIDController> discGrabPID = std::make_shared<IterativePosPIDController>(discGrabGains, TimeUtilFactory::withSettledUtilParams(2, 2, 200_ms));

		pros::Mutex motorThreadSafety;

		QLength relativeTrackingDistance = 0.0 * inch;
		double PIDvelocityLimit = 1.0;

	public:
		QPath::Settings settings;

		ExtendedChassis (
			const MotorGroup& ileft, const MotorGroup& iright, 
			const AbstractMotor::GearsetRatioPair& igearset, const ChassisScales& iscales,
			const ADIEncoder& right, const ADIEncoder& middle, const RotationSensor leftVelocity, const RotationSensor rightVelocity, const IMU& inertial1, const IMU& inertial2, const std::int8_t distance,
			const IterativePosPIDController::Gains& idistanceGains, const IterativePosPIDController::Gains& iturnGains, const IterativePosPIDController::Gains& iangleGains, const IterativePosPIDController::Gains& idistanceSensorGains,
			const QPath::Settings set
		);

		std::shared_ptr<ChassisModel> getModel();

		void startOdom();
		void stopOdom();

		void followNewPath(QPath path);

		void setOdomPos(QPoint ipos, QAngle iheading);

		void turnToPoint(QPoint ipoint, QTime time, bool ireversed = false);

		void turnToAngle(QAngle targetAngle, QTime time);
		void driveDistance(QLength target, QTime time);
		void driveToDistanceFrom(QLength target, QTime time);
		void discGrabOscilations(QLength target, QTime time);

		void setPIDVelocityLimit(double velLimit);
	};

}