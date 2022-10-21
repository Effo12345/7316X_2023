#include "main.h"

bool fwToggle = false;
int intakeToggle = false;
bool curvatureToggle = false;

bool autonSelectorActive = true;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {
	//Initialize PROS LCD
	//pros::lcd::initialize();
	selector.init();
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
		//Button to turn the flywheel on and toggle the indexer
		if(master[ControllerDigital::L1].changedToPressed()) {
			//Deactivate the auton selector and start the grapher
			if(autonSelectorActive) {
				selector.setActive(false);
				grapher.initGraph();
				autonSelectorActive = false;
			}
			
			if(fwToggle) {
				//Actuate the indexer if the flywheel is already moving
				indexer.index();
			}
			else {
				//Otherwise, turn the flywheel on
				fwToggle = true;
				fw.setVelocity(600);
			}
		}

		//Button to turn the flywheel off
		if(master[ControllerDigital::L2].changedToPressed()) {
			fwToggle = false;
			fw.setVelocity(0);
		}

		//Button to toggle disc intake
		if(master[ControllerDigital::R1].changedToPressed())
			intakeToggle = !intakeToggle;

		//Button for inverting the disc intake
		if(master[ControllerDigital::A].changedToPressed())
			intakeToggle *= -1;

		//Button to toggle between tank control and curvature control
		if(master[ControllerDigital::Y].changedToPressed())
			curvatureToggle = !curvatureToggle;

		//Button to set the roller mech
		if(master[ControllerDigital::R2].isPressed()) {
			roller.moveVelocity(200);
		}
		else {
			roller.moveVelocity(0);
		}
		

		//Using okapilib's chassis object, set the wheel velocity based on the sticks and the selected control scheme
		if(curvatureToggle) {
			chassis->getModel()->curvature(master.getAnalog(ControllerAnalog::leftY),
										 master.getAnalog(ControllerAnalog::rightX));
		}
		else {
			chassis->getModel()->tank(master.getAnalog(ControllerAnalog::leftY),
									master.getAnalog(ControllerAnalog::rightY));
		}

		//Set the power of the intake based on value calculated above
		intake.moveVelocity(100 * intakeToggle);

		//Control flywheel velocity based on value set above
		if(!autonSelectorActive)
			fw.controlVelocity();
		
		pros::delay(20);
	}
}
