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
        chassis->driveDistance(-8_in, 1_s);
        pros::delay(100);
        rollerMech.flip();
        pros::delay(100);
        chassis->driveDistance(3_in, 1_s);
        everythingElse.moveVoltage(12000);
        pros::delay(100);

        chassis->turnToAngle(-45_deg, 1_s);
        fw.moveVelocity(3000, 0.9);
        pros::delay(100);
        chassis->driveDistance(5_in, 1_s);
        pros::delay(100);

        chassis->driveDistance(0_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(48_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(15_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(-14_deg, 1_s);
        pros::delay(2000);

        everythingElse.moveVoltage(-6000);
        pros::delay(2000);
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

    //Programming skills: 9 discs in high goal, 4 rollers, expansion
    void Skills() {
        std::pair<int, float> matchloadVel {1990, 0.86};

        chassis->setOdomPos({0.1875, -56.5}, 180_deg);
        chassis->startOdom();
        fw.toggleReverse();
        chassis->driveDistance(7_in, 250_ms);
        (chassis->getModel())->arcade(0.1, 0.0);
        pros::delay(1000);
        (chassis->getModel())->stop();
        everythingElse.moveVoltage(12000);
        chassis->driveDistance(0_in, 500_ms);
        pros::delay(100);
        fw.toggleReverse();

        fw.moveVelocity(matchloadVel);
        chassis->turnToPoint({53.01, -57.57}, 1_s);
        pros::delay(100);
        chassis->driveDistance(6_in, 1_s);
        pros::delay(2000);
        everythingElse.moveVelocity(-12000);
        pros::delay(1000);

        fw.toggleReverse();
        everythingElse.moveVoltage(0);
        chassis->driveDistance(0_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(180_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(4_in, 250_ms);
        (chassis->getModel())->arcade(0.1, 0.0);
        pros::delay(3000);
        (chassis->getModel())->stop();
        everythingElse.moveVoltage(12000);

        chassis->driveDistance(-2_in, 500_ms);
        pros::delay(100);
        fw.toggleReverse();

        fw.moveVelocity(matchloadVel);
        chassis->turnToPoint({53.01, -57.57}, 1_s);
        pros::delay(100);
        chassis->driveDistance(6_in, 1_s);
        pros::delay(3000);
        everythingElse.moveVelocity(-12000);
        pros::delay(1125);

        fw.toggleReverse();
        everythingElse.moveVoltage(0);
        chassis->driveDistance(0_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(179_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(4_in, 250_ms);
        (chassis->getModel())->arcade(0.1, 0.0);
        pros::delay(3000);
        (chassis->getModel())->stop();
        everythingElse.moveVoltage(12000);

        chassis->driveDistance(-2_in, 500_ms);
        pros::delay(100);
        fw.toggleReverse();

        fw.moveVelocity(matchloadVel);
        chassis->turnToPoint({53.01, -57.57}, 1_s);
        pros::delay(100);
        chassis->driveDistance(6_in, 1_s);
        pros::delay(2000);
        everythingElse.moveVelocity(-12000);
        pros::delay(1000);
        fw.moveVelocity(0);

        chassis->turnToAngle(95_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(-28_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(0_deg, 1_s);
        pros::delay(100);
        (chassis->getModel())->arcade(-0.5, 0.0);
        pros::delay(500);
        (chassis->getModel())->stop();
        pros::delay(100);
        rollerMech.skillsFlip2();

        chassis->driveDistance(12_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(90_deg, 1_s);
        pros::delay(100);
        //chassis->driveDistance(-28_in, 1_s);
        (chassis->getModel())->arcade(-0.5, 0.0);
        pros::delay(1250);
        (chassis->getModel())->arcade(-0.1, 0.0);
        rollerMech.skillsFlip2();
        (chassis->getModel())->stop();

        chassis->driveDistance(0_in, 1_s);
        pros::delay(100);
        chassis->turnToAngle(45_deg, 1_s);
        pros::delay(100);
        chassis->driveDistance(-16_in, 1_s);
        pros::delay(100);
        
        expansion.toggle();
        chassis->stopOdom();
    }