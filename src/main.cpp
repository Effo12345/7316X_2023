#include "main.h"
#include "globals.hpp"
#include "pros/llemu.hpp"
#include "xlib/chassis/extendedchassis.hpp"

//Boolean flags for use in driver control
/*
bool fwToggle = false;
int intakeToggle = false;
bool rollerToggle = false;
bool indexerToggle = false;
bool curvatureToggle = false;
bool autonSelectorActive = true;
bool hasExpanded = false;
bool adjusterState = false;
*/
bool fwToggle = false;
int intakeState = false;
bool adjusterState = false;
bool hasExpanded = false;

//Holds the current target velocities for the flywheel
std::pair<int, float> flywheelVel {2425, 0.866};
std::pair<int, float> angledFlywheelVel = {2500,0.886};
std::pair<int, float> maxFlywheelVel {3600, 1.0};
std::pair<int, float> flywheelVelocitiesL1[2] {flywheelVel, angledFlywheelVel};

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
	selector.setActive(false);
	pros::lcd::initialize();
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {	
	//Run the auton the user selected based on the output of the Selector class
	//selector.runSelection();

	chassis->startOdom({11.62, -65.34}, 0_deg);
	chassis->settings.reversed = false;
	chassis->followNewPath({{
		{11.62, -65.34},
        {-15.87, -41.24},
        {-13.04, 2.13},
        {38.83, 0.71},
        {16.72, 57.4}
	}});
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while(true) {
		if(master[ControllerDigital::L1].changedToPressed() && !fwToggle) {
			fw.moveVelocity(flywheelVelocitiesL1[adjusterState]);
		}
		else if(master[ControllerDigital::L1].isPressed() && fwToggle) {
			primary.setNormalizedVelocity(-1.0);
		}
		
		if(master[ControllerDigital::L1].changedToReleased()) {
			fwToggle = true;
			primary.setNormalizedVelocity(intakeState);
		}

		if(master[ControllerDigital::L2].changedToPressed()) {
			fwToggle = false;
			fw.moveVelocity(0);
		}

		if(master[ControllerDigital::R1].changedToPressed()) {
			intakeState = !intakeState;
			primary.setNormalizedVelocity(intakeState);
		}

		if(master[ControllerDigital::R2].changedToPressed()) {
			primary.setNormalizedVelocity(1.0);
		}
		else if(master[ControllerDigital::R2].changedToReleased()) {
			intakeState = false;
			primary.setNormalizedVelocity(0);
		}

		if(master[ControllerDigital::A].changedToPressed()) {
			primary.setNormalizedVelocity(intakeState *= -1);
		}

		if(master[ControllerDigital::up].changedToPressed()) {
			adjusterState = !adjusterState;
			angleAdjuster.toggle();
			fw.moveVelocity(flywheelVelocitiesL1[adjusterState]);
		}

		if(master[ControllerDigital::X].changedToPressed()) {
			fw.moveVelocity(maxFlywheelVel);
			fwToggle = true;
		}

		if(master[ControllerDigital::L1].isPressed() &&
		   master[ControllerDigital::L2].isPressed() &&
		   master[ControllerDigital::R1].isPressed() &&
		   master[ControllerDigital::R2].isPressed() &&
		   !hasExpanded) {
			hasExpanded = true;
			primary.setNormalizedVelocity(intakeState = false);
			fw.moveVelocity(fwToggle = false);
			expansion.toggle();
		   }

		chassis->getModel()->tank (
			master.getAnalog(ControllerAnalog::leftY),
			master.getAnalog(ControllerAnalog::rightY)
		);

		if(master[ControllerDigital::left].changedToPressed())
			primary.staggeredIndex(3, 1000_ms);

		pros::delay(20);
	}
}


/*
if l1 (changed pressed) && !flywheel toggle
	start flywheel;
	flywheelToggle = true;
else if l1(changed to released) && flywheel toggle
	readyToIndex = true;


if l1 (changed to pressed) && readyToIndex
	set negative;
else if l1 (changed to released) && readyToIndex
	set intakeToggle;

if r1 (changed to pressed)
	intakeToggle = !intakeToggle;
	set intakeToggle;

if r2 (changed to pressed)
	set positive;
else if r2 (changed to released)
	intakeToggle = false;
	set stop;

if l1 && l2 && r1 && r2 && hasExpanded
	hasExpanded = true;
	expansion.toggle;

	
*/
