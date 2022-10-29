#include "xlib/extendedchassis.hpp"

/*
 * The class ExtendedChassis holds all autonomous movement functions. It uses
 * the OdomChassisController declared in globals.cpp to power the wheels based
 * on the output of the PID functions.
 */

namespace xlib {
//Convert between degrees of tracking wheel rotation and inches of robot movement
double ExtendedChassis::DegToIn(double deg) {
    return deg / wheelConversionFactor;
}

double ExtendedChassis::InToDeg(double in) {
	return in * wheelConversionFactor;
}

//Turn to face angle iAngle using inertial sensor PID
void ExtendedChassis::turnTo(double iAngle ) {
    do {
        turnPID.setTarget(iAngle);

		double imu = gyro.get();
		double power = turnPID.step(imu);
		(chassis->getModel())->arcade(0, power);
		//pros::lcd::set_text(1, std::to_string(gyro.get()));
		//pros::lcd::set_text(2, std::to_string(power));
		pros::delay(25);
	} while((!turnPID.isSettled()));
    (chassis->getModel())->arcade(0, 0);
    leftEncoder.reset();
}

//Drive until length iDistance using tracking wheel PID
void ExtendedChassis::driveTo(double iDistance) {
    do {
        drivePID.setTarget(InToDeg(iDistance));

		double distance = leftEncoder.get();
		double power = drivePID.step(distance);
		(chassis->getModel())->arcade(power, 0);
		//pros::lcd::set_text(1, std::to_string(DegToIn(distance)));
		//pros::lcd::set_text(2, std::to_string(power));
        std::cout << distance << ", " << power <<"\n";
		pros::delay(25);
    } while(!drivePID.isSettled());
    (chassis->getModel())->arcade(0, 0);
}

//A variant of driveTo with alternate PID tuning for short movements
void ExtendedChassis::driveShortTo(double iDistance) {
    do {
        driveShortPID.setTarget(InToDeg(iDistance));

		double distance = leftEncoder.get();
		double power = driveShortPID.step(distance);
		(chassis->getModel())->arcade(power, 0);
		//pros::lcd::set_text(1, std::to_string(DegToIn(distance)));
		//pros::lcd::set_text(2, std::to_string(power));
        std::cout << distance << ", " << power <<"\n";
		pros::delay(25);
    } while(!driveShortPID.isSettled());
    (chassis->getModel())->arcade(0, 0);
}

ExtendedChassis control;

}