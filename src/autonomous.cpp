#include "autonomous.hpp"
#include "globals.hpp"

    //Turns the roller on the left side of the field 
    void WPL() {
        //Back into the roller
        chassis->getModel()->arcade(-0.5, 0);
        pros::delay(500);
        chassis->getModel()->arcade(0, 0);

        //Flip the roller to scored
        chassis->getModel()->arcade(-0.1, 0);
	    rollerMech.flip();
        chassis->getModel()->arcade(0, 0);
    }

    //Turns a roller and shoots three discs into the high goal
    void FullL() {
        //Back into the roller and flip it to scored
        fw.moveVelocity(0);
        chassis->driveDistance(-3_in, 500_ms);
        rollerMech.flip();
        chassis->driveDistance(2_in, 500_ms);
        pros::delay(100);
        everythingElse.moveVoltage(12000);
        fw.toggleReverse();

        //Turn and drive to grab the disc on the auton line
        chassis->turnToAngle(-34_deg, 1000_ms);
        pros::delay(100);
        chassis->driveDistance(8_in, 1000_ms);
        chassis->driveDistance(-1_in, 1000_ms);
        fw.toggleReverse();
        pros::delay(1000);

        //Spin up flywheel and drive to optimal shooting spot
        fw.moveVelocity(2530, 0.6);
        chassis->turnToAngle(57_deg, 500_ms);
        pros::delay(100);
        chassis->driveDistance(24_in, 1000_ms);
        pros::delay(100);
        chassis->turnToAngle(-10.5_deg, 1000_ms);
        pros::delay(100);
        chassis->driveDistance(7_in, 500_ms);
        pros::delay(2000);

        //Shoot into the high goal
        everythingElse.moveVoltage(-6000);
        pros::delay(2500);
        everythingElse.moveVoltage(0);
        fw.moveVelocity(0);
    }

    //Turns the roller on the right half of the field for half of the autonomous
    //win point
    void WPR() {
        //Navigate to the roller
        chassis->driveDistance(-15_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(90_deg, 1000_ms);
        pros::delay(100);

        chassis->driveDistance(-11_in, 500_ms);
        pros::delay(100);

        //Flip the roller to a scored position
        rollerMech.flip();
    }

    //Programming skills: 9 discs in high goal, 4 rollers, expansion
    void Skills() {
        //Score the starting roller
        fw.moveVelocity(0);
        chassis->driveDistance(-8_in, 500_ms);
        rollerMech.skillsFlip();
		chassis->turnToAngle(0_deg, 0_ms);
        chassis->driveDistance(2_in, 500_ms);
        pros::delay(100);

        //Grab a disc on the auton line
        chassis->turnToAngle(-32_deg, 500_ms);
		everythingElse.moveVoltage(12000);
        pros::delay(100);
        chassis->driveDistance(10_in, 1000_ms);
        pros::delay(1000);

        //Navigate to second roller
        chassis->turnToAngle(0_deg, 500_ms);
        pros::delay(100);

        chassis->driveDistance(12_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(90_deg, 1000_ms);
        pros::delay(100);
        everythingElse.moveVoltage(0);

        //Flip roller to scored
        chassis->driveDistance(-24_in, 1000_ms);
        rollerMech.skillsFlip();
        pros::delay(100);

        //Drive to high goal, ensuring discs are fully within the intake
        chassis->driveDistance(-10_in, 1500_ms);
        fw.toggleReverse();
        everythingElse.moveVoltage(12000);
        pros::delay(100);
        chassis->turnToAngle(0_deg, 1000_ms);
        pros::delay(100);
        fw.toggleReverse();
        fw.moveVelocity(2230, 0.42);
        chassis->driveDistance(40_in, 3500_ms);
        pros::delay(100);

        //Point at the high goal and shoot a volley
        chassis->turnToAngle(13_deg, 500_ms);
        pros::delay(2500);
        everythingElse.moveVoltage(-12000);
        pros::delay(1000);
        everythingElse.moveVoltage(0);
        pros::delay(250);
        fw.moveVelocity(0);
        pros::delay(100);

        chassis->turnToAngle(-4_deg, 500_ms);
        pros::delay(100);
        chassis->driveDistance(-50_in, 3500_ms);
        pros::delay(100);

        chassis->turnToAngle(45_deg, 1500_ms);
        pros::delay(100);
		chassis->driveDistance(25_in, 500_ms);
        
        expansion.toggle();
    }