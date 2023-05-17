#include "autonomous.hpp"

void left6Disc() {
    chassis->startOdom({-36.875, -58.375}, 0_deg);

	(chassis->getModel())->arcade(-0.5, 0);
	pros::delay(200);
	(chassis->getModel())->arcade(0, 0);
	primary.rollerFlip();
	(chassis->getModel())->arcade(0.25, 0);
	pros::delay(150);
	(chassis->getModel())->stop();
	primary.setNormalizedVelocity(1);
	
	chassis->settings.kV = 3.8;
	chassis->followNewPath({{
		{-38.27, -67.32},
        {-43.94, -52.3}
	}});
	fw.moveVelocity({2800, 0.9});

	chassis->settings.reversed = true;
	chassis->followNewPath({{
		{-44.79, -51.45},
        {-29.48, -45.21}
	}});

	chassis->turnToAngle(-18_deg, 1.5_s);
	pros::delay(1500);
	primary.staggeredIndex(3, 500_ms);
	pros::delay(100);
	fw.moveVelocity(2600, 0.8);

	chassis->getModel()->arcade(-0.5, 0);
	pros::delay(150);
	chassis->getModel()->stop();

	chassis->turnToAngle(45_deg, 750_ms);

	chassis->settings.reversed = false;
	chassis->settings.useRateLimiter = false;
	chassis->settings.maxAcceleration = 400;
	chassis->followNewPath({{
		{-29.48, -45.21},
        {-21.54, -43.23},
        {-15.59, -38.98}
	}});

	primary.setNormalizedVelocity(1);

	chassis->settings.useRateLimiter = true;
	chassis->settings.maxAcceleration = 200;
	chassis->settings.maxVelocity = 40;
	chassis->followNewPath({{
		{-16.44, -42.66},
        {-11.06, -33.59},
        {-6.52, -19.7}
	}});

	chassis->turnToAngle(-44_deg, 1_s);
	primary.staggeredIndex(3, 500_ms);

    fw.moveVelocity(0);
    chassis->stopOdom();
}


void left3Disc() {
    chassis->startOdom({-36.875, -58.375}, 0_deg);

	(chassis->getModel())->arcade(-0.5, 0);
	pros::delay(200);
	(chassis->getModel())->arcade(0, 0);
	primary.rollerFlip();
	(chassis->getModel())->arcade(0.25, 0);
	pros::delay(150);
	(chassis->getModel())->stop();
	primary.setNormalizedVelocity(1);
	
	chassis->settings.kV = 3.8;
	chassis->followNewPath({{
		{-38.27, -67.32},
        {-43.94, -52.3}
	}});
	fw.moveVelocity({2700, 0.7});

	chassis->settings.reversed = true;
	chassis->followNewPath({{
		{-44.79, -51.45},
        {-29.48, -45.21}
	}});

	chassis->turnToAngle(-18_deg, 1.5_s);
	(chassis->getModel())->arcade(0.25, 0);
	pros::delay(600);
	chassis->getModel()->stop();
	pros::delay(2400);
	primary.staggeredIndex(3, 1000_ms);
	fw.moveVelocity(0);

    chassis->stopOdom();
}


void leftRoller() {
    (chassis->getModel())->arcade(-0.5, 0);
	pros::delay(200);
	(chassis->getModel())->arcade(0, 0);
	primary.rollerFlip2();
}


void right6Disc() {
    chassis->startOdom({55.9375, 6.25}, -90_deg);
	fw.moveVelocity({2750, 0.9});

	primary.setNormalizedVelocity(1);
	chassis->followNewPath({{
		{54.71, 7.23},
        {39.12, 11.48},
        {29.2, 14.6}
	}});

	pros::delay(1250);
	primary.staggeredIndex(3, 300_ms);

	primary.setNormalizedVelocity(1);
	chassis->turnToAngle(-30_deg, 500_ms);
	pros::delay(100);

	(chassis->getModel())->arcade(0.4, 0.0);
	pros::delay(200);
	(chassis->getModel())->stop();

	(chassis->getModel())->arcade(-0.4, 0.0);
	pros::delay(250);
	(chassis->getModel())->stop();

	fw.moveVelocity({2700, 0.88});
	chassis->turnToAngle(-138_deg, 500_ms);
	
	chassis->followNewPath({{
		{36, 11.76},
        {23.24, -1.56},
        {9.35, -8.65}
	}});

	pros::delay(100);
	chassis->turnToAngle(-44_deg, 1000_ms);

	primary.staggeredIndex(3, 300_ms);

	chassis->turnToAngle(-135_deg, 500_ms);
	fw.moveVelocity(0);
	
	
	chassis->settings.reversed = true;
	chassis->settings.maxAcceleration = 300;
	chassis->settings.maxRateChange = 300;
	chassis->settings.lookaheadDistance = 20;
	chassis->followNewPath({{
		{12.76, -11.76},
        {51.87, 37.28}
	}});

	chassis->turnToAngle(-90_deg, 400_ms);
	(chassis->getModel())->arcade(-0.7, 0);
	pros::delay(150);
	chassis->getModel()->stop();
	primary.rollerFlip2();
	(chassis->getModel())->stop();

    chassis->stopOdom();
}


void right3Disc() {
    chassis->startOdom({55.9375, 6.25}, -90_deg);
	fw.moveVelocity({2500, 0.7});

	primary.setNormalizedVelocity(1);
	chassis->followNewPath({{
		{54.71, 7.23},
        {39.12, 11.48},
        {29.2, 14.6}
	}});

	pros::delay(3000);
	primary.staggeredIndex(3, 1000_ms);
	fw.moveVelocity(0);
	
	chassis->turnToAngle(-135_deg, 1_s);
	pros::delay(100);

	chassis->settings.reversed = true;
	chassis->followNewPath({{
		{29.2, 14.6},
        {56.69, 35.01}
	}});
	pros::delay(100);

	chassis->turnToAngle(-90_deg, 1_s);
	(chassis->getModel())->arcade(-0.7, 0);
	pros::delay(150);
	chassis->getModel()->stop();
	primary.rollerFlip2();
	(chassis->getModel())->stop();

    chassis->stopOdom();
}


void rightRoller() {
    chassis->startOdom({55.9375, 6.25}, -90_deg);
	chassis->turnToAngle(-180_deg, 1_s);
	pros::delay(100);

	chassis->settings.reversed = true;
	chassis->followNewPath({{
		{55.94, 6.25},
        {54.99, 35.57}
	}});
	pros::delay(100);

	chassis->turnToAngle(-90_deg, 1_s);
	pros::delay(100);

	chassis->getModel()->arcade(-0.5, 0);
	pros::delay(200);
	chassis->getModel()->stop();
	primary.rollerFlip();
	(chassis->getModel())->stop();

    chassis->stopOdom();
}


void fullWP() {
    chassis->startOdom({-36.875, -58.375}, 0_deg);
	fw.moveVelocity(2600, 0.82);

	(chassis->getModel())->arcade(-0.5, 0);
	pros::delay(100);
	(chassis->getModel())->arcade(0, 0);
	primary.rollerFlip2();
	chassis->getModel()->stop();

	chassis->settings.maxAcceleration = 300;
	chassis->settings.lookaheadDistance = 15;
	chassis->settings.kV = 3.5;
	chassis->followNewPath({{
		{-41.67, -65.91},
        {-33.73, -46.91},
        {-18.71, -41.53}
	}});

	primary.setNormalizedVelocity(1);

	chassis->settings.maxAcceleration = 200;
	chassis->settings.maxVelocity = 50;
	chassis->settings.k = 5.0;
	chassis->settings.lookaheadDistance = 10;
	chassis->followNewPath({{
		{-17.29, -36.71},
        {4.25, -49.46},
        {9.92, -38.41},
        {-4.25, -18.57}
	}});

	chassis->turnToAngle(-46_deg, 500_ms);
	primary.staggeredIndex(3, 300_ms);

	fw.moveVelocity(2800, 0.87);
	chassis->turnToAngle(70_deg, 400_ms);
	primary.setNormalizedVelocity(1);

	chassis->settings.k = 4.0;
	chassis->settings.maxVelocity = 200;
	chassis->settings.maxAcceleration = 150;
	chassis->settings.lookaheadDistance = 15;
	chassis->followNewPath({{
		{-4.25, -18.57},
        {8.79, -6.09},
        {31.75, 6.94},
        {33.45, 16.87}
	}});

	chassis->turnToAngle(-76_deg, 750_ms);
	primary.staggeredIndex(3, 300_ms);
	fw.moveVelocity(0);

	chassis->turnToAngle(-135_deg, 200_ms);
	chassis->settings.reversed = true;
	chassis->settings.k = 7.0;
	chassis->settings.lookaheadDistance = 10;
	chassis->settings.maxAcceleration = 250;
	chassis->followNewPath({{
		{33.45, 16.87},
        {43.65, 27.64},
        {47.91, 32.74}
	}});
	chassis->turnToAngle(-90_deg, 400_ms);
	chassis->getModel()->arcade(-0.8, 0);
	pros::delay(200);
	chassis->getModel()->stop();
	primary.rollerFlip2();

    chassis->stopOdom();
}