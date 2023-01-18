#include "main.h"

//Boolean flags for use in driver control
bool fwToggle = false;
int intakeToggle = false;
bool rollerToggle = false;
bool indexerToggle = false;
bool curvatureToggle = false;
bool autonSelectorActive = true;
bool hasExpanded = false;

//Holds the current target velocity for the flywheel
int flywheelVel = 2128;
int cornerFlywheelVel = 2130;

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {}

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

	/*
	control.driveTo(-3_in, 500_ms);
	rollerMech.flip();
	control.driveTo(2_in, 500_ms);
	pros::delay(100);
	everythingElse.moveVoltage(12000);

	control.turnTo(-32_deg, 500_ms);
	pros::delay(100);
	control.driveTo(7_in, 1000_ms);
	control.driveTo(-1_in, 1000_ms);
	pros::delay(1000);

	fw.moveVelocity(2475, 0.6);
	control.turnTo(57_deg, 500_ms);
	pros::delay(100);
	control.driveTo(24_in, 1000_ms);
	pros::delay(100);
	control.turnTo(-10.5_deg, 1000_ms);
	pros::delay(100);
	control.driveTo(7_in, 500_ms);

	everythingElse.moveVoltage(-6000);
	pros::delay(2000);
	everythingElse.moveVoltage(0);
	fw.moveVelocity(0);
	*/

	fw.moveVelocity(0);
	control.driveTo(-8_in, 500_ms);
	rollerMech.skillsFlip();
	control.driveTo(2_in, 500_ms);
	pros::delay(100);
	everythingElse.moveVoltage(12000);

	control.turnTo(-32_deg, 500_ms);
	pros::delay(100);
	control.driveTo(10_in, 1000_ms);
	pros::delay(1000);

	control.turnTo(0_deg, 500_ms);
	pros::delay(100);

	control.driveTo(18_in, 1_s);
	pros::delay(100);
	control.turnTo(90_deg, 1000_ms);
	pros::delay(100);
	everythingElse.moveVoltage(0);

	control.driveTo(-22_in, 1000_ms);
	//(chassis->getModel())->arcade(-0.05, 0.0);
	rollerMech.skillsFlip();
	//(chassis->getModel())->stop();
	pros::delay(100);


	control.driveTo(-14_in, 1500_ms);
	fw.toggleReverse();
	everythingElse.moveVoltage(12000);
	pros::delay(100);
	control.turnTo(0_deg, 1000_ms);
	pros::delay(100);
	fw.toggleReverse();
	fw.moveVelocity(1997, 0.4);
	control.driveTo(56_in, 3500_ms);
	pros::delay(100);

	control.turnTo(16_deg, 500_ms);
	pros::delay(1500);
	everythingElse.moveVoltage(-12000);
	pros::delay(1000);
	everythingElse.moveVoltage(0);
	pros::delay(250);
	fw.moveVelocity(0);

	control.turnTo(83_deg, 1_s);
	pros::delay(100);

	everythingElse.moveVoltage(12000);
	(chassis->getModel())->arcade(0.2, 0.0);
	pros::delay(4750);
	(chassis->getModel())->stop();
	fw.moveVelocity(2115, 0.4);
	pros::delay(1000);
	//everythingElse.moveVoltage(0);

	control.driveTo(-2_in, 1_s);
	pros::delay(100);
	control.turnTo(16_deg, 1_s);
	pros::delay(100);

	everythingElse.moveVoltage(-12000);
	pros::delay(1000);
	everythingElse.moveVoltage(0);
	pros::delay(250);
	fw.moveVelocity(0);

	control.turnTo(-15_deg, 1_s);
	pros::delay(100);
	control.driveTo(-48_in, 1.5_s);
	pros::delay(100);
	control.turnTo(47_deg, 1_s);
	pros::delay(100);

	fw.moveVelocity(2115, 0.41);
	everythingElse.moveVoltage(12000);
	(chassis->getModel())->arcade(0.27, 0.0);
	pros::delay(4250);
	(chassis->getModel())->stop();
	pros::delay(250);
	control.turnTo(-42_deg, 1.5_s);
	pros::delay(500);

	everythingElse.moveVoltage(-12000);
	pros::delay(1500);
	everythingElse.moveVoltage(0);
	pros::delay(250);
	fw.moveVelocity(0);

	/*
	control.turnTo(-8_deg, 500_ms);
	pros::delay(100);
	control.driveTo(-63_in, 2_s);
	pros::delay(100);
	control.turnTo(45_deg, 1_s);
	pros::delay(100);
	control.driveTo(4_in, 500_ms);
	pros::delay(1000);
	*/
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
		//Button to activate the indexer
		if(master[ControllerDigital::L1].isPressed()) {
			if(fwToggle)
				indexerToggle = true;
		}

		//Button to turn the flywheel on and toggle the indexer
		if(master[ControllerDigital::L1].changedToPressed()) {
			if(!fwToggle) {
				//Otherwise, turn the flywheel on at  rpm
				//fwToggle = true;
				fw.moveVelocity(flywheelVel, 0.405);
			}
		}
		else if(master[ControllerDigital::L1].changedToReleased())
			fwToggle = true;

		//Button to set the flywheel to corner speed and toggle indexer
		//Button to turn the flywheel on and toggle the indexer
		if(master[ControllerDigital::right].changedToPressed()) {
			if(!fwToggle) {
				//Otherwise, turn the flywheel on at  rpm
				//fwToggle = true;
				fw.moveVelocity(cornerFlywheelVel, 0.405);
			}
		}
		else if(master[ControllerDigital::right].changedToReleased())
			fwToggle = true;

		//Button to turn the flywheel off
		if(master[ControllerDigital::L2].changedToPressed()) {
			fwToggle = false;
			fw.moveVelocity(0);
		}

		//Reverse the flywheel and intake in case a disc gets stuck
		if(master[ControllerDigital::down].changedToPressed()) {
			intakeToggle = true;
			fw.toggleReverse();
		}
		else if(master[ControllerDigital::down].changedToReleased()) {
			intakeToggle = false;
			fw.toggleReverse();
		}

		
		if(master[ControllerDigital::up].changedToPressed())
			autonomous();


		//Button to toggle disc intake
		if(master[ControllerDigital::R1].changedToPressed())
			intakeToggle = !intakeToggle;

		//Button to set the roller mech
		if(master[ControllerDigital::R2].isPressed()) {
			rollerToggle = true;
			intakeToggle = false;
		}
		else {
			rollerToggle = false;
		}

		//Trigger expansion by pressing all 4 shoulder buttons
		//This ensures it will never be triggered accidentally
		if( !hasExpanded &&
			master[ControllerDigital::L1].isPressed() &&
			master[ControllerDigital::L2].isPressed() &&
			master[ControllerDigital::R1].isPressed() &&
			master[ControllerDigital::R2].isPressed() ) {
				expansion.toggle();
				hasExpanded = true;
				fwToggle = false;
				rollerToggle = false;
				fw.moveVelocity(0);	
				intakeToggle = false;
				master.rumble("-");
		} 

		//Button for inverting the disc intake
		if(master[ControllerDigital::A].changedToPressed())
			intakeToggle *= -1;

		//Button to toggle between tank control and curvature control
		if(master[ControllerDigital::Y].changedToPressed())
			curvatureToggle = !curvatureToggle;

		//Using okapilib's chassis object, set the wheel velocity based on 
		//the sticks and the selected control scheme
		if(curvatureToggle) {
			chassis->getModel()->curvature (
				master.getAnalog(ControllerAnalog::leftY),
				master.getAnalog(ControllerAnalog::rightX)
			);
		}
		else {
			chassis->getModel()->tank (
				master.getAnalog(ControllerAnalog::leftY),
				master.getAnalog(ControllerAnalog::rightY)
			);
		}

		//Set the power of the everythingElse motor based on values calculated above
		if(indexerToggle)
			everythingElse.moveVoltage(-12000);
		else if(intakeToggle || intakeToggle == -1)
			everythingElse.moveVoltage(12000 * intakeToggle);
		else if(rollerToggle)
			everythingElse.moveVoltage(12000);
		else
		 everythingElse.moveVoltage(0);

		indexerToggle = false;
		
		pros::delay(20);
	}
}
