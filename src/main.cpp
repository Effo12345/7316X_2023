#include "main.h"
#include "globals.hpp"

//Boolean flags for use in driver control
bool fwToggle = false;
int intakeState = false;
bool adjusterState = false;
bool hasExpanded = false;

//Holds the current target velocities for the flywheel
std::pair<int, float> flywheelVel {2600, 0.866};
std::pair<int, float> angledFlywheelVel = {2700,0.9};
std::pair<int, float> maxFlywheelVel {3600, 1.0};
std::pair<int, float> flywheelVelocitiesL1[2] {flywheelVel, angledFlywheelVel};

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
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
	selector.runSelection();
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
	primary.stop();

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
			primary.setNormalizedVelocity(-1.0);
		}
		else if(master[ControllerDigital::R2].changedToReleased()) {
			intakeState = false;
			primary.stop();
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
			lowExpansion1.toggle();
			lowExpansion2.toggle();
			highExpansion.toggle();
		}

		if(master[ControllerDigital::down].changedToPressed() && !hasExpanded) {
			lowExpansion1.toggle();
			lowExpansion2.toggle();
		}


		chassis->getModel()->tank (
			master.getAnalog(ControllerAnalog::leftY),
			master.getAnalog(ControllerAnalog::rightY)
		);

		pros::delay(20);
	}
}
