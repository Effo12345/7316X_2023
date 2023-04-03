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

        Velocity operator-(const Velocity& v) {
            return {leftVel - v.leftVel, rightVel - v.rightVel};
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

    QPos pos {{0, 0}, 0};
    float headingOffset {0.0f};
    Velocity vel;

    ADIEncoder tracking {'Z', 'Z'};
    IMU imu {0};
    RotationSensor leftRotation {0};

    double rightEncoderDistance;
    double middleEncoderDistance;
    double wheelRadius;

    double prevRightEncoderPos = 0.0;
    double prevMiddleEncoderPos = 0.0;
    double prevHeading = 0.0;

    double degToRad(double deg);
    double radToDeg(double rad);
    double degToIn(double deg);
    double DPSToRPM(double dps);

    double average(std::vector<double> n);

    void loop();

public:
    QPos getPos();
    QPos getRawPos();
    void setPos(QPoint ipos, QAngle iheading);

    QLength getTrack();

    double getInternalIMU();

    void withSensors(const ADIEncoder& itracking, const IMU& inertial);

    void setScales(QLength iscales);

    void startLoop();
    void stopLoop();
};

}