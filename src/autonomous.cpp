#include "autonomous.hpp"
#include "globals.hpp"

    //Turns the roller on the left side of the field 
    void WPL() {
        chassis->startOdom();
        //Back into the roller
        chassis->getModel()->arcade(-0.5, 0);
        pros::delay(500);
        chassis->getModel()->arcade(0, 0);

        //Flip the roller to scored
        chassis->getModel()->arcade(-0.1, 0);
	    rollerMech.flip();
        chassis->getModel()->arcade(0, 0);
        chassis->stopOdom();
    }

    //Turns a roller and shoots three discs into the high goal
    void FullL() {
        chassis->startOdom();
        fw.moveVelocity(3100, 0.88);
        //Back into the roller
        (chassis->getModel())->arcade(-0.5, 0);
        pros::delay(250);
        (chassis->getModel())->arcade(0, 0);

        //Flip the roller to scored
        (chassis->getModel())->arcade(-0.1, 0);
        rollerMech.flip();
        (chassis->getModel())->stop();

        //Drive to the 3 stack of discs
        chassis->driveDistance(6_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(56_deg, 1_s);
        pros::delay(100);
        
        everythingElse.moveVoltage(12000);
        chassis->driveDistance(22_in, 5_s);
        //Oscilate to intake the stack more effectively
        chassis->setPIDVelocityLimit(0.9);
        chassis->discGrabOscilations(48_in, 1_s);
        pros::delay(200);
        //Navigate to face the high goal
        chassis->turnToAngle(-29_deg, 1_s);
        pros::delay(1000);

        //Shoot a volley
        everythingElse.moveVoltage(-12000);
        pros::delay(190);
        everythingElse.moveVoltage(0);
        pros::delay(500);

        for(int i = 0; i < 3; i++) {
            everythingElse.moveVoltage(-12000);
            pros::delay(500);
            everythingElse.moveVoltage(0);
            pros::delay(500);
        }

        chassis->stopOdom();
    }

    //Turns the roller on the right half of the field for half of the autonomous
    //win point
    void WPR() {
        chassis->startOdom();
        //Navigate to the roller
        chassis->driveDistance(-15_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(90_deg, 1000_ms);
        pros::delay(100);

        chassis->driveDistance(-11_in, 500_ms);
        pros::delay(100);

        //Flip the roller to a scored position
        rollerMech.flip();
        chassis->stopOdom();
    }
    
    void FullR() {
        chassis->startOdom();
        chassis->setPIDVelocityLimit(0.7);
        //Drive forward and intake a disc. Navigate to face the high goal
        fw.moveVelocity(2600, 0.85);
        everythingElse.moveVoltage(12000);
        chassis->driveDistance(24_in, 2_s);
        pros::delay(100);
        chassis->turnToAngle(33_deg, 1_s);
        pros::delay(2000);

        //Shoot a volley
        everythingElse.moveVoltage(-12000);
        pros::delay(190);
        everythingElse.moveVoltage(0);
        pros::delay(500);

        for(int i = 0; i < 3; i++) {
            everythingElse.moveVoltage(-12000);
            pros::delay(500);
            everythingElse.moveVoltage(0);
            pros::delay(500);
        }

        //Navigate to the roller
        fw.moveVelocity(0);
        chassis->turnToAngle(-45_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(-35_in, 2_s);
        pros::delay(100);
        chassis->turnToAngle(0_deg, 1_s);
        pros::delay(100);

        //Flip the roller to scored
        (chassis->getModel())->arcade(-0.4, 0.0);
        pros::delay(500);
        (chassis->getModel())->stop();
        rollerMech.flip();
        chassis->stopOdom();
    }

    void FullWP() {
        chassis->startOdom();
        fw.moveVelocity(3100, 0.88);
        //Back into the roller
        (chassis->getModel())->arcade(-0.5, 0);
        pros::delay(250);
        (chassis->getModel())->arcade(0, 0);

        //Flip the roller to scored
        (chassis->getModel())->arcade(-0.1, 0);
        rollerMech.flip();
        (chassis->getModel())->stop();

        //Navigate to the stack of discs
        chassis->driveDistance(6_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(56_deg, 1_s);
        pros::delay(100);
        
        everythingElse.moveVoltage(12000);
        chassis->driveDistance(22_in, 5_s);
        chassis->setPIDVelocityLimit(0.9);
        //Oscilte the intake to grab discs more efficiently
        chassis->discGrabOscilations(48_in, 1_s);
        pros::delay(200);
        //Navigate to face the high goal and shoot a volley
        chassis->turnToAngle(-29_deg, 1_s);
        pros::delay(100);
        everythingElse.moveVelocity(-38);
        chassis->driveDistance(8_in, 500_ms);
        pros::delay(500);
        chassis->driveDistance(6_in, 1_s);

        //Grab the line of 3 discs
        fw.moveVelocity(3450, 0.94);
        chassis->turnToAngle(50_deg, 1_s);
        pros::delay(100);
        everythingElse.moveVoltage(12000);
        chassis->setPIDVelocityLimit(0.7);
        chassis->driveDistance(70_in, 5_s);
        pros::delay(100);
        chassis->setPIDVelocityLimit(0.9);
        chassis->turnToAngle(-70_deg, 1_s);
        //Shoot a second volley
        everythingElse.moveVelocity(-38);
        pros::delay(1700);

        //Navigate to the second roller
        chassis->setPIDVelocityLimit(1.0);
        chassis->turnToAngle(-155_deg, 1_s);
        fw.moveVelocity(0);
        chassis->driveDistance(-14_in, 1_s);
        chassis->turnToAngle(-90_deg, 1_s);
        chassis->driveDistance(-11_in, 1.5_s);
        //Flip the roller to scored
        rollerMech.flip();
        chassis->stopOdom();
    }

    //Programming skills: 9 discs in high goal, 4 rollers, expansion
    void Skills() {
        //Store the desired flywheel velocity
	    std::pair<int, float> matchloadVel {2250, 0.91};

        chassis->setOdomPos({0.1875, -56.5}, 180_deg);
        chassis->startOdom();
		chassis->setPIDVelocityLimit(0.7);

		//Intake the first volley of matchloads
        fw.toggleReverse();
        chassis->driveDistance(7_in, 250_ms);
        (chassis->getModel())->arcade(0.1, 0.0);
        pros::delay(3000);
        (chassis->getModel())->stop();
        everythingElse.moveVoltage(12000);
        chassis->driveDistance(0_in, 500_ms);
		fw.toggleReverse();
        pros::delay(100);

		//Turn to face the high goal and shoot a volley
        fw.moveVelocity(matchloadVel);
        chassis->turnToPoint({53.01, -55.57}, 1_s);
        pros::delay(100);
        chassis->driveDistance(8_in, 1_s);
        pros::delay(3000);
        everythingElse.moveVelocity(-12000);
        pros::delay(1000);

		//Navigate back to the match loader and intake the second volley
		fw.moveVelocity(0);
        fw.toggleReverse();
        everythingElse.moveVoltage(0);
        chassis->driveDistance(1_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(180_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(7_in, 500_ms);
        (chassis->getModel())->arcade(0.1, 0.0);
        pros::delay(3500);
        (chassis->getModel())->stop();
        everythingElse.moveVoltage(12000);

		//Turn to face the high goal and shoot second volley
        chassis->driveDistance(-2_in, 500_ms);
		fw.toggleReverse();
        pros::delay(100);
        fw.moveVelocity(matchloadVel);
        chassis->turnToPoint({53.01, -55.57}, 1_s);
        pros::delay(100);
        chassis->driveDistance(8_in, 1_s);
        pros::delay(3500);
        everythingElse.moveVelocity(-12000);
        pros::delay(1125);

		//Navigate back to the match loader and intke volley 3
		fw.moveVelocity(0);
        fw.toggleReverse();
        everythingElse.moveVoltage(0);
        chassis->driveDistance(2_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(179_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(7_in, 500_ms);
        (chassis->getModel())->arcade(0.1, 0.0);
        pros::delay(3000);
        (chassis->getModel())->stop();
        everythingElse.moveVoltage(12000);

		//Turn to face the high goal and shoot the third volley
        chassis->driveDistance(0_in, 500_ms);
		fw.toggleReverse();
        pros::delay(100);
        fw.moveVelocity(matchloadVel);
        chassis->turnToPoint({53.01, -55.57}, 1_s);
        pros::delay(100);
        chassis->driveDistance(8_in, 1_s);
        pros::delay(2000);
        everythingElse.moveVelocity(-12000);
        pros::delay(1000);
        fw.moveVelocity(0);

		//Navigate to the first roller
        chassis->turnToAngle(95_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(-43_in, 3_s);
        pros::delay(100);
        chassis->turnToAngle(0_deg, 2_s);
		everythingElse.moveVoltage(0);
        pros::delay(100);
        (chassis->getModel())->arcade(-0.5, 0.0);
        pros::delay(500);
        (chassis->getModel())->stop();
        pros::delay(100);
		//Flip roller to scored
        rollerMech.skillsFlip2();

		//Navigate to the second roller, knocking discs out of the way
        chassis->driveDistance(18_in, 3_s);
        pros::delay(100);
        chassis->turnToAngle(90_deg, 1_s);
        pros::delay(100);
        (chassis->getModel())->arcade(-0.5, 0.0);
        pros::delay(1100);
        (chassis->getModel())->arcade(-0.15, 0.0);
		//Flip the roller to scored
        rollerMech.skillsFlip();
        (chassis->getModel())->stop();

		//Drive across the field to score the third roller
		chassis->driveDistance(0_in, 1_s);
		pros::delay(100);
		chassis->turnToAngle(43_deg, 1_s);
		pros::delay(100);
		chassis->driveDistance(116_in, 7_s);
		pros::delay(100);
		chassis->driveDistance(105_in, 1_s);
		chassis->turnToAngle(180_deg, 1_s);
		pros::delay(100);

		//Back into the roller and flip it to scored
		chassis->driveDistance(-22_in, 1.5_s);
		(chassis->getModel())->arcade(-0.15, 0.0);
		pros::delay(100);
		rollerMech.skillsFlip();
		(chassis->getModel())->stop();

		//Navigate to the fourth roller
		chassis->driveDistance(18_in, 2_s);
		pros::delay(100);
		chassis->driveDistance(13_in, 2_s);
		pros::delay(100);
		chassis->turnToAngle(-90_deg, 2_s);
		pros::delay(100);
		(chassis->getModel())->arcade(-0.5, 0.0);
        pros::delay(1250);
        (chassis->getModel())->arcade(-0.15, 0.0);
		//Flip it to scored
        rollerMech.skillsFlip();
        (chassis->getModel())->stop();
		pros::delay(200);

		//Navigate to the ideal expansion location
		chassis->driveDistance(21_in, 750_ms);
		chassis->turnToAngle(225_deg, 1_s);
		chassis->driveDistance(-19_in, 500_ms);

		//Expand
		expansion.toggle();
		chassis->stopOdom();
    }

    void SafetySkills() {
        chassis->startOdom();
        //Limit the robot to half speed
        chassis->setPIDVelocityLimit(0.5);
        (chassis->getModel())->arcade(-0.2, 0.0);
        pros::delay(1000);
        //Flip the first roller to scored
        rollerMech.skillsFlip2();
        pros::delay(100);

        //Navigate to the second roller
        chassis->driveDistance(5_in, 2_s);
        pros::delay(200);
        everythingElse.moveVoltage(12000);
        chassis->turnToAngle(-45_deg, 2_s);
        pros::delay(200);
        chassis->driveDistance(27_in, 5_s);
        pros::delay(200);
        chassis->turnToAngle(90_deg, 2_s);
        everythingElse.moveVoltage(0);
        pros::delay(200);

        //Flip the roller to scored
        (chassis->getModel())->arcade(-0.2, 0.0);
        pros::delay(1500);
        rollerMech.skillsFlip2();
        (chassis->getModel())->stop();
        pros::delay(100);

        //Navigate through the low goal
        chassis->driveDistance(-2_in, 2_s);
        pros::delay(200);
        chassis->turnToAngle(-1_deg, 3_s);
        pros::delay(200);
        chassis->driveDistance(90_in, 10_s);
        pros::delay(500);
        chassis->turnToAngle(89_deg, 2_s);
        pros::delay(200);

        //Drive to the third roller
        chassis->driveDistance(40_in, 10_s);
        pros::delay(500);
        chassis->turnToAngle(94_deg, 2_s);
        pros::delay(200);
        chassis->driveDistance(50_in, 8_s);
        pros::delay(500);
        chassis->turnToAngle(178_deg, 2_s);
        pros::delay(200);
        (chassis->getModel())->arcade(-0.2, 0.0);
        pros::delay(1500);
        //Flip the third roller to scored
        rollerMech.skillsFlip2();
        (chassis->getModel())->stop();
        pros::delay(100);

        //Navigate to the fourth roller
        chassis->driveDistance(0_in, 2_s);
        pros::delay(200);
        everythingElse.moveVoltage(12000);
        chassis->turnToAngle(135_deg, 2_s);
        pros::delay(200);
        chassis->driveDistance(27_in, 5_s);
        pros::delay(200);
        chassis->turnToAngle(270_deg, 2_s);
        everythingElse.moveVoltage(0);
        pros::delay(200);

        (chassis->getModel())->arcade(-0.2, 0.0);
        pros::delay(1500);
        //Flip the fourth roller to scored
        rollerMech.skillsFlip2();
        (chassis->getModel())->stop();
        pros::delay(100);

        //Navigate to ideal expansion location
        chassis->driveDistance(15_in, 3_s);
        chassis->turnToAngle(225_deg, 1_s);
        chassis->driveDistance(-17_in, 3_s);

        //Expand
        expansion.toggle();
        chassis->stopOdom();
    }