#pragma once
#include "globals.hpp"

namespace xlib {
    class ExtendedChassis {
		const double trackingCircumference = 2.75 * 3.141592653589; //Circumference of tracking wheel
		const double wheelConversionFactor = 360 / trackingCircumference;

    public:
        void turnTo(double iAngle);

        void driveTo(double iDistance);
    
		void driveShortTo(double iDistance);

		double DegToIn(double deg);
		double InToDeg(double in);
    };
	
	extern ExtendedChassis control;
}