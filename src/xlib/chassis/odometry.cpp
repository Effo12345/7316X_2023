#include "xlib/chassis/odometry.hpp"
#include "okapi/api/util/mathUtil.hpp"

namespace xlib {

    /**
     * Returns the input degrees in radians
     */
    double Odom::degToRad(double deg) {
        return deg * (okapi::pi / 180);
    }

    /**
     * Returns the input radians in degrees
     */
    double Odom::radToDeg(double rad) {
        return rad / (okapi::pi / 180);
    }

    /**
     * Returns the input wheel degrees to translational inches
     */
    double Odom::degToIn(double deg) {
        return degToRad(deg * wheelRadius);
    }

    /**
     * Returns the input degrees/second in rpm
     */
    double Odom::DPSToRPM(double dps) {
        return dps / 0.166667;
    }   

    /**
     * Calculate the change in the robot's absolute position (cartesian, (x,y)
     * coordinates) using the sensor data from two tracking wheels and an 
     * inertial sensor. Edits the class-scoped variable pos which holds the
     * robot's absolute position. The loop in this function runs asynchronously
     * by inheriting from the TaskWrapper class.
     */
    void Odom::loop() {
        while(true) {
            //Get encoder values
            double rPos = rightEncoder.get();
            double sPos = middleEncoder.get();

            //Convert distance traveled by each wheel in inches since the previous loop
            double deltaDistR = degToIn(rPos - prevRightEncoderPos);
            double deltaDistS = degToIn(sPos - prevMiddleEncoderPos);

            //Update previous encoder values
            prevRightEncoderPos = rPos;
            prevMiddleEncoderPos = sPos;

            //Calculate current absolute heading in radians
            double heading = degToRad(imu.get());

            //Calculate change in angle
            double deltaTheta = heading - prevHeading;
            prevHeading = heading;

            QPoint localDelta {0, 0};
            //If the robot hasn't turned, then it only translated
            //Prevents a divide by zero error
            if(deltaTheta == 0) {
                localDelta.y = deltaDistR;
            }
            //Otherwise, calculate the new position
            else {
                //Calculate the changes in x and y
                //Distance = 2 * radius * sin(theta / 2)
                localDelta.x = 2 * sin(deltaTheta / 2.0) * ((deltaDistS / deltaTheta) + middleEncoderDistance);
                localDelta.y = 2 * sin(deltaTheta / 2.0) * ((deltaDistR / deltaTheta) - rightEncoderDistance);
            }

            //Find the average angle the robot follows in its arc
            double avgTheta = heading - (deltaTheta / 2.0);

            QPoint globalDelta {0, 0};
            //Rotate the local delta into global coordinates
            globalDelta.x = (localDelta.x * cos(avgTheta)) - (localDelta.y * sin(avgTheta));
            globalDelta.y = (localDelta.x * sin(avgTheta)) + (localDelta.y * cos(avgTheta));

            //Update global position values
            //Take a mutex to ensure the pos values aren't being written to 
            //another thread is attempting to read them
            posThreadSafety.take();
            pos.p.x += globalDelta.x;
            pos.p.y += globalDelta.y;
            pos.a = deltaTheta;
            posThreadSafety.give();

            pros::delay(10);
        }
    }

    /**
     * Accesses the class-scoped variable pos, converts its heading value to
     * degrees and uses a mutex for thread safety.
     *
     * @return Global position struct
     */
    Odom::QPos Odom::getPos() {
        //Returned by value rather than by reference for thread safety
        posThreadSafety.take();
        QPos tmp = pos;
        posThreadSafety.give();

        pos.a = radToDeg(pos.a);
        return tmp;
    }

    /**
     * Returns the value of the inertial sensor for debugging
     */
    double Odom::getInternalIMU() {
        return imu.get();
    }

    /**
     * Fetches drivetrain wheel velocities and converts their degrees/second to
     * rpm
     *
     * @return Measured velocity struct
     */
    Odom::Velocity Odom::getVel() {
        return {DPSToRPM(leftRotation.getVelocity()), DPSToRPM(rightRotation.getVelocity())};
    }

    /**
     * Configuration function used to initialize an instance of the Odom class.
     * Sets the sensors to be used for measurement
     *
     * @param right Right tracking wheel encoder, parallel to drive wheels
     * @param middle Middle tracking wheel encoder, perpendicular to drive
     * @param inertial Inertial sensor, measures robot yaw
     * @param leftVel Measures velocity of left side of drivetrain
     * @param rightVel Measures velocity of right side of drivetrain
     */
    void Odom::withSensors(const ADIEncoder& right, const ADIEncoder& middle, const IMU& inertial, const RotationSensor& leftVel, const RotationSensor& rightVel) {
        rightEncoder = right;
        middleEncoder = middle;
        imu = inertial;
        leftRotation = leftVel;
        rightRotation = rightVel;
    }

    /**
     * Initializes distance constants used in odom math
     *
     * @param iscales Dimensions of tracking wheel placement and size
     */
    void Odom::setScales(const ChassisScales& iscales) {
        rightEncoderDistance = iscales.wheelTrack.convert(inch);
        middleEncoderDistance = iscales.middleWheelDistance.convert(inch);
        wheelRadius = iscales.wheelDiameter.convert(inch) / 2;
    }

    /**
     * Starts the internal thread (calls loop asynchronously)
     */
    void Odom::startLoop() {
        startTask();
    }

}