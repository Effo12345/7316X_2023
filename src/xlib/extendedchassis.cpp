#include "xlib/extendedchassis.hpp"
#include "globals.hpp"

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
void ExtendedChassis::turnTo(QAngle targetAngle, QTime time) {
	turnPID->reset();
	turnPID->setTarget(0);
	turnPID->setIntegratorReset(true);
	turnPID->setIntegralLimits(0.4 / 0.015, -0.4 / 0.015);
	turnPID->setErrorSumLimits(15, 0);

	auto timer = TimeUtilFactory().createDefault().getTimer();
	timer->placeMark();

	do {
		(chassis->getModel())->arcade(0, turnPID->step(-rescale180(targetAngle.convert(degree)-gyro.get())));
		pros::lcd::set_text(1, std::to_string(gyro.get()));
		pros::delay(10);
	}while(!turnPID->isSettled() && timer->getDtFromMark() < time);

	(chassis->getModel())->stop();
	(chassis->getModel())->resetSensors();
}

//Drive until length iDistance using tracking wheel PID
void ExtendedChassis::driveTo(QLength target, QTime time) {
    movePID->reset();
	headingPID->reset();

	movePID->setTarget(0);
	headingPID->setTarget(gyro.get());

	auto timer = TimeUtilFactory().createDefault().getTimer();
	timer->placeMark();

	do {
		double distance = tickToIn((static_cast<double>((chassis->getModel())->getSensorVals()[0] + (chassis->getModel())->getSensorVals()[1]) / 2), chassis->getChassisScales(), chassis->getGearsetRatioPair());
		double error = target.convert(inch) - distance;
		(chassis->getModel())->arcade(movePID->step(-error), headingPID->step(gyro.get()));
		pros::lcd::set_text(1, std::to_string(gyro.get()));
		pros::delay(10);
	}while(!movePID->isSettled() && timer->getDtFromMark() < time);

	(chassis->getModel())->stop();
}

ExtendedChassis control;
}
