#include "autonomous.hpp"
#include "globals.hpp"

    //Turns the roller on the left side of the field 
    void WPL() {
        //Back into the roller
        chassis->getModel()->arcade(-0.5, 0);
        pros::delay(100);
        chassis->getModel()->arcade(0, 0);

        //Flip the roller to scored
        chassis->getModel()->arcade(0.05, 0);
	    rollerMech.flip();
        chassis->getModel()->arcade(0, 0);
    }

    void FullL() {
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

        everythingElse.moveVoltage(-12000);
        pros::delay(250);
        everythingElse.moveVoltage(0);
        fw.moveVelocity(2400);
        control.turnTo(-13.5_deg, 500_ms);
        pros::delay(750);

        everythingElse.moveVoltage(-12000);
        pros::delay(250);
        everythingElse.moveVoltage(0);
        fw.moveVelocity(0);
    }

    //Turns the roller on the right half of the field for half of the autonomous
    //win point
    void WPR() {
        //Navigate to the roller
        control.driveTo(30_in, 1_s);
        pros::delay(100);
        control.turnTo(90_deg, 1000_ms);
        pros::delay(100);
        
        chassis->getModel()->arcade(0.15, 0);
	    pros::delay(1000);
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);

        //Flip the roller to a scored position
        chassis->getModel()->arcade(0.27, 0);
	    rollerMech.skillsFlip2();
        chassis->getModel()->arcade(0, 0);
    }

    //Programming skills. Scores two rollers and expands at the 1o second mark
    void Skills() {
        //Back into the roller
        chassis->getModel()->arcade(-0.5, 0);
        pros::delay(100);
        chassis->getModel()->arcade(0, 0);

        //Flip the roller to scored
        chassis->getModel()->arcade(0.05, 0);
	    rollerMech.skillsFlip();
        chassis->getModel()->arcade(0, 0);
	
		chassis->getModel()->arcade(0.5, 0);
	    pros::delay(800);
        chassis->getModel()->arcade(0, 0);
		pros::delay(100);

		control.turnTo(45_deg, 1000_ms);
		pros::delay(200);

	    chassis->getModel()->arcade(-0.25, 0);
        pros::delay(1000);
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);

		expansion.toggle();
    }