#pragma once
#include "okapi/impl/device/rotarysensor/rotationSensor.hpp"
#include "xlib/taskwrapper.hpp"
#include "xlib/chassis/point.hpp"
#include "okapi/api.hpp"

using namespace okapi;

namespace xlib {

class Odom: TaskWrapper {
public:
     struct QPos {
	    QPoint p {0, 0};
	    float a {0};
    };

    struct Velocity {
        double leftVel;
        double rightVel;

        Velocity operator+(const Velocity& v) {
		    return {leftVel + v.leftVel, rightVel + v.rightVel};
	    } 

        Velocity operator*(const float f) {
		return {leftVel * f, rightVel * f};
        }

        Velocity operator/(const float f) {
		return {leftVel / f, rightVel / f};
	    }
    };

private:
    pros::Mutex posThreadSafety;

    QPos pos;
    Velocity vel;

    ADIEncoder rightEncoder {'Z', 'Z'};
    ADIEncoder middleEncoder {'Z', 'Z'};
    IMU imu {0};
    RotationSensor leftRotation {0};
    RotationSensor rightRotation {0};

    double rightEncoderDistance;
    double middleEncoderDistance;
    double wheelRadius;

    double prevRightEncoderPos;
    double prevMiddleEncoderPos;
    double prevHeading;

    double degToRad(double deg);
    double radToDeg(double rad);
    double degToIn(double deg);
    double DPSToRPM(double dps);

    void loop();

public:
    QPos getPos();

    Velocity getVel();

    double getInternalIMU();

    void withSensors(const ADIEncoder& right, const ADIEncoder& middle, const IMU& inertial, const RotationSensor& leftVel, const RotationSensor& rightVel);

    void setScales(const ChassisScales& iscales);

    void startLoop();
};

}