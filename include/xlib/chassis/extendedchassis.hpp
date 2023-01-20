#pragma once
//#include "main.h"
#include "okapi/api/control/iterative/iterativePosPidController.hpp"
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

		pros::Mutex motorThreadSafety;

	public:
		QPath::Settings settings;

		ExtendedChassis (
			const MotorGroup& ileft, const MotorGroup& iright, 
			const AbstractMotor::GearsetRatioPair& igearset, const ChassisScales& iscales,
			const ADIEncoder& right, const ADIEncoder& middle, const RotationSensor leftVelocity, const RotationSensor rightVelocity, const IMU& inertial, 
			const IterativePosPIDController::Gains& idistanceGains, const IterativePosPIDController::Gains& iturnGains, const IterativePosPIDController::Gains& iangleGains,
			const QPath::Settings set
		);

		std::shared_ptr<ChassisModel> getModel();

		void followNewPath(QPath path);

		void turnToAngle(QAngle targetAngle, QTime time);
		void driveDistance(QLength target, QTime time);
	};

}