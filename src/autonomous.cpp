#include "autonomous.hpp"
#include "globals.hpp"

    //Turns the roller on the left side of the field 
    void WPL() {
        //Back into the roller
        chassis->getModel()->arcade(0.5, 0);
        pros::delay(100);
        chassis->getModel()->arcade(0, 0);

        //Flip the roller to scored
        chassis->getModel()->arcade(0.05, 0);
	    rollerMech.flip();
        chassis->getModel()->arcade(0, 0);
    }

    //Turns the roller on the right half of the field for half of the autonomous
    //win point
    void WPR() {
        //Navigate to the roller
        control.driveTo(24);
        pros::delay(100);
        control.turnTo(90);
        pros::delay(100);
        control.driveTo(5);

        //Flip the roller to a scored position
        chassis->getModel()->arcade(0.5, 0);
	    rollerMech.flip();
        chassis->getModel()->arcade(0, 0);
    }

    //Programming skills. Scores two rollers and expands at the 1o second mark
    void Skills() {
        fw.init();
        fw.setVelocity(435);

        //Back into the roller
        chassis->getModel()->arcade(0.5, 0);
        pros::delay(100);
        chassis->getModel()->arcade(0, 0);

        //Flip the roller to scored
        chassis->getModel()->arcade(0.05, 0);
        rollerMech.skillsFlip();
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);

        //Back away from the roller
        control.driveTo(-5);
        pros::delay(100);
        //Turn to face the high goal
        control.turnTo(-11);
        pros::delay(100);

        //Wait for the flywheel to spin up, then launch a disc
        pros::delay(7000);
        indexer.index();
        fw.setVelocity(352);
        //Wait for the flywheel to stabilize and launch a disc
        pros::delay(4000);
        indexer.index();
        pros::delay(100);
        fw.setVelocity(0);

        //Drive to second roller
        control.driveTo(-14);
        pros::delay(100);
        control.turnTo(90);
        pros::delay(100);

        //Turn on intake to clear disc in path
        intake.moveVelocity(200);
        //Flip the roller to scored
        chassis->getModel()->arcade(0.5, 0);
        pros::delay(1000);
        intake.moveVelocity(0);
        pros::delay(500);
        rollerMech.skillsFlip();
        chassis->getModel()->arcade(0, 0);
        rightEncoder.reset();
        pros::delay(100);
    }