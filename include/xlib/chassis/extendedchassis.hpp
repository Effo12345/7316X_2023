#pragma once
#include "xlib/chassis/odometry.hpp"
#include "xlib/chassis/pathgenerator.hpp"
#include "xlib/chassis/pathfollower.hpp"

namespace xlib {

	class ExtendedChassis {
		std::shared_ptr<ChassisController> drive;
		Odom odometer;
		PathGenerator pathGenerator;
		PathFollower pathFollower;

		std::shared_ptr<IterativePosPIDController> turnPID;
		std::shared_ptr<IterativePosPIDController> movePID;
		std::shared_ptr<IterativePosPIDController> headingPID;

		std::shared_ptr<MotorGroup> leftVelocityGetter;
		std::shared_ptr<MotorGroup> rightVelocityGetter;

		pros::Mutex motorThreadSafety;

		QLength relativeTrackingDistance = 0.0 * inch;
		double PIDvelocityLimit = 1.0f;


		Odom::Velocity getVel();

	public:
		QPath::Settings settings;

		ExtendedChassis (
			const MotorGroup& ileft, const MotorGroup& iright, 
			const AbstractMotor::GearsetRatioPair& igearset, const ChassisScales& iscales,
			const ADIEncoder& tracking, const IMU& inertial,
			const IterativePosPIDController::Gains& idistanceGains, const IterativePosPIDController::Gains& iturnGains, const IterativePosPIDController::Gains& iangleGains,
			const QPath::Settings set
		);

		std::shared_ptr<ChassisModel> getModel();

		void startOdom(QPoint ipos = {0, 0}, QAngle iheading = 0_deg);
		void stopOdom();

		void followNewPath(QPath path);

		void turnToPoint(QPoint ipoint, QTime time, bool ireversed = false);

		void turnToAngle(QAngle targetAngle, QTime time);
		void driveDistance(QLength target, QTime time);

		void setPIDVelocityLimit(double velLimit);
	};

}