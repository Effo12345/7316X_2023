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
    Velocity vel;

    ADIEncoder rightEncoder {'Z', 'Z'};
    ADIEncoder middleEncoder {'Z', 'Z'};
    IMU imu1 {0};
    IMU imu2 {0};
    RotationSensor leftRotation {0};
    RotationSensor rightRotation {0};

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

    QLength getRightTrack();

    Velocity getVel();

    double getInternalIMU();

    void withSensors(const ADIEncoder& right, const ADIEncoder& middle, const IMU& inertial1, const IMU& inertial2, const RotationSensor& leftVel, const RotationSensor& rightVel);

    void setScales(const ChassisScales& iscales);

    void startLoop();
    void stopLoop();
};

}