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
        chassis->turnToAngle(-33_deg, 1000_ms);
        pros::delay(100);
        chassis->driveDistance(8_in, 1000_ms);
        chassis->driveDistance(-1_in, 1000_ms);
        fw.toggleReverse();
        pros::delay(1000);

        //Spin up flywheel and drive to optimal shooting spot
        fw.moveVelocity(2355, 0.6);
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
        chassis->driveDistance(-25_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(90_deg, 1000_ms);
        pros::delay(100);
        
        chassis->driveDistance(-10_in, 500_ms);
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
        chassis->driveDistance(2_in, 500_ms);
        pros::delay(100);
        everythingElse.moveVoltage(12000);

        //Grab a disc on the auton line
        chassis->turnToAngle(-32_deg, 500_ms);
        pros::delay(100);
        chassis->driveDistance(10_in, 1000_ms);
        pros::delay(1000);

        //Navigate to second roller
        chassis->turnToAngle(0_deg, 500_ms);
        pros::delay(100);

        chassis->driveDistance(18_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(90_deg, 1000_ms);
        pros::delay(100);
        everythingElse.moveVoltage(0);

        //Flip roller to scored
        chassis->driveDistance(-24_in, 1000_ms);
        rollerMech.skillsFlip();
        pros::delay(100);

        //Drive to high goal, ensuring discs are fully within the intake
        chassis->driveDistance(-12_in, 1500_ms);
        fw.toggleReverse();
        everythingElse.moveVoltage(12000);
        pros::delay(100);
        chassis->turnToAngle(0_deg, 1000_ms);
        pros::delay(100);
        fw.toggleReverse();
        fw.moveVelocity(2180, 0.4);
        chassis->driveDistance(45_in, 3500_ms);
        pros::delay(100);

        //Point at the high goal and shoot a volley
        chassis->turnToAngle(10_deg, 500_ms);
        pros::delay(1500);
        everythingElse.moveVoltage(-12000);
        pros::delay(1000);
        everythingElse.moveVoltage(0);
        pros::delay(250);
        fw.moveVelocity(0);
        chassis->driveDistance(13_in, 1000_ms);

        //Intake the line of discs adjacent to the low goal barrier
        chassis->turnToAngle(80_deg, 1_s);
        pros::delay(100);

        everythingElse.moveVoltage(12000);
        (chassis->getModel())->arcade(0.2, 0.0);
        pros::delay(4750);
        (chassis->getModel())->stop();
        fw.moveVelocity(2150, 0.4);
        pros::delay(1000);

        //Navigate back to the high goal
        chassis->driveDistance(-2_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(12_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(-7_in, 500_ms);
        pros::delay(100);

        //Shoot a volley
        everythingElse.moveVoltage(-12000);
        pros::delay(1000);
        everythingElse.moveVoltage(0);
        pros::delay(250);
        fw.moveVelocity(0);

        //Intake the line of three discs
        chassis->driveDistance(0_in, 500_ms);
        pros::delay(100);
        chassis->turnToAngle(-15_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(-42_in, 1.5_s);
        pros::delay(100);
        chassis->turnToAngle(47_deg, 1_s);
        pros::delay(100);

        fw.moveVelocity(2050, 0.41);
        everythingElse.moveVoltage(12000);
        (chassis->getModel())->arcade(0.27, 0.0);
        pros::delay(4250);
        (chassis->getModel())->stop();
        pros::delay(500);
        //Point at the high goal
        chassis->turnToAngle(-44_deg, 1.5_s);
        pros::delay(500);
        chassis->driveDistance(10_in, 500_ms);
        pros::delay(250);

        //Shoot a volley
        everythingElse.moveVoltage(-12000);
        pros::delay(1500);
        everythingElse.moveVoltage(0);
        pros::delay(250);
        fw.moveVelocity(0);
        chassis->driveDistance(0_in, 500_ms);

        //Drive to third roller, knocking over and pushing aside a stack of discs
        chassis->turnToAngle(-135_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(-33_in, 3_s);
        pros::delay(100);
        chassis->turnToAngle(-120_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(-50_in, 2000_ms);
        pros::delay(100);

        chassis->driveDistance(-40_in, 500_ms);
        pros::delay(500);
        chassis->turnToAngle(-180_deg, 1000_ms);
        pros::delay(200);
        (chassis->getModel())->arcade(-0.5, 0.0);
        pros::delay(750);
        (chassis->getModel())->stop();
        //Flip the roller to scored
        rollerMech.skillsFlip();
        pros::delay(100);

        //Navigate to the fourth roller
        chassis->driveDistance(2_in, 500_ms);
        pros::delay(100);
        everythingElse.moveVoltage(12000);

        chassis->turnToAngle(148_deg, 500_ms);
        pros::delay(100);
        chassis->driveDistance(10_in, 1000_ms);
        pros::delay(1000);

        chassis->turnToAngle(180_deg, 500_ms);
        pros::delay(100);

        chassis->driveDistance(14_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(270_deg, 1000_ms);
        pros::delay(100);
        everythingElse.moveVoltage(0);

        //Flip the roller to scored
        chassis->driveDistance(-25_in, 1000_ms);
        pros::delay(100);
        rollerMech.skillsFlip();
        pros::delay(100);

        //Drive to the optimal expansion spot
        (chassis->getModel())->resetSensors();
        chassis->driveDistance(33_in, 500_ms);
        pros::delay(100);
        chassis->turnToAngle(225_deg, 500_ms);
        pros::delay(100);
        chassis->driveDistance(-20_in, 500_ms);
        pros::delay(100);

        //Expand
        expansion.toggle();
    }