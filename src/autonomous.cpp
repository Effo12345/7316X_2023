#include "autonomous.hpp"
#include "globals.hpp"

    //Turns the roller on the left side of the field and launches preload discs
    //for half of the autonomous win point
    void WPL() {
        //Back into the roller
        control.driveShortTo(-1);

        //Flip the roller to scored
	    rollerMech.flip();

        //Spin up the flywheel
	    //fw.init();
	    //fw.setVelocity(600);
        flyWheel.moveVelocity(600);
        control.driveTo(4);
        pros::delay(100);

        //Turn to face the high goal
        control.turnTo(-8);
        pros::delay(1000);

        //Launch two discs
        indexer.index();
        pros::delay(2000);
        indexer.index();
        pros::delay(500);
        flyWheel.moveVelocity(0);
    }

    //Turns the roller on the right half of the field for half of the autonomous
    //win point
    void WPR() {
        //Turn -90 degrees
        control.turnTo(-90);
        pros::delay(100);

        //Drive backwards
        chassis->getModel()->arcade(-0.5, 0);
        pros::delay(1000);
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);

        //Turn to face starting orientation
        control.turnTo(0);
        pros::delay(100);
        
        //Back up into the roller
        chassis->getModel()->arcade(-0.5, 0);
        pros::delay(300);
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);

        //Flip the roller to a scored position
        rollerMech.flip();
    }

    //Programming skills. Scores two rollers and expands at the 1o second mark
    void Skills() {
        //Back into roller
        control.driveShortTo(-1);

        //Flip the roller to a scored position
        chassis->getModel()->arcade(0.1, 0);
        rollerMech.flip();
        chassis->getModel()->arcade(0, 0);

        //Drive forward to knock disc out of the way
        chassis->getModel()->arcade(0.5, 0);
        pros::delay(1000);
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);

        //Turn to face second roller
        control.turnTo(75);
        pros::delay(100);

        //Drive backwards to avoid disc
        chassis->getModel()->arcade(-0.5, 0);
        pros::delay(1000);
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);

        //Turn to face second roller
        control.turnTo(90);
        pros::delay(100);

        //Drive backwards into second roller
        chassis->getModel()->arcade(-0.5, 0);
        pros::delay(400);
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);	

        //Flip the second roller into a scored position
        chassis->getModel()->arcade(0.1, 0);
        rollerMech.flip();
        chassis->getModel()->arcade(0, 0);

        //Drive forward away from roller
        chassis->getModel()->arcade(0.5, 0);
        pros::delay(1000);
        chassis->getModel()->arcade(0, 0);
        pros::delay(100);	

        //Turn to an angle in preparation for expansion
        control.turnTo(135);
    }