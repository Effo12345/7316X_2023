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
     * Return the average of values in the input vector
     *
     * @param n Vector of values to be averaged
     */
    double Odom::average(std::vector<double> n) {
        double runningSum;

        for(double val : n)
            runningSum += val;

        return runningSum / n.size();
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
            //Wait for the inertial sensor to finish calibrating. Continue until
            //it does
            if(std::isnan(imu1.get()) || std::isnan(imu2.get()))  
                continue;


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
            double heading = degToRad(360 - imu1.get());

            //Calculate change in angle
            double deltaTheta = heading - prevHeading;
            prevHeading = heading;

            QPoint localDelta {0, 0};
            //If the robot hasn't turned, then it only translated
            //Prevents a divide by zero error
            if(deltaTheta == 0) {
                localDelta.x = deltaDistS;
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
            globalDelta.x = (localDelta.y * cos(avgTheta)) - (localDelta.x * sin(avgTheta));
            globalDelta.y = (localDelta.y * sin(avgTheta)) + (localDelta.x * cos(avgTheta));

            //Update global position values
            //Take a mutex to ensure the pos values aren't being written to 
            //while another thread is attempting to read them
            posThreadSafety.take();
            pos.p += globalDelta;
            pos.a += deltaTheta;
            posThreadSafety.give();

            pros::lcd::set_text(0, std::to_string(pos.p.y * -1));
            pros::lcd::set_text(1, std::to_string(pos.p.x));
            pros::lcd::set_text(2, std::to_string((radToDeg(pos.a) - 360) * -1));

            pros::delay(10);
        }
    }

    /**
     * Accesses the class-scoped variable pos, using a mutex for thread 
     * safety
     *
     * @return Global position struct (inch, inch, radian)
     */
    Odom::QPos Odom::getPos() {
        //Returned by value rather than by reference for thread safety
        posThreadSafety.take();
        QPos tmp = pos;
        posThreadSafety.give();

        //Take the opposite of y and heading values because odometry internally
        //uses left turn is positive, while pure pursuit and PID turns use
        //left turn is negative.
        tmp.p.y *= -1;
        tmp.a -= (2 * okapi::pi);
        tmp.a *= -1;

        float storage = tmp.p.x;
        tmp.p.x = tmp.p.y;
        tmp.p.y = storage;

        return tmp;
    }

    /**
     * Return the position in its original coordinates (left turn is positive,
     * swap x and y)
     *
     * @return Unprocessed global position struct (inch, inch, radian)
     */
    Odom::QPos Odom::getRawPos() {
        posThreadSafety.take();
        QPos tmp = pos;
        posThreadSafety.give();

        return tmp;
    }

    /**
     * Set the current absolute position and rotation for the start of an
     * autonomous procedure
     *
     * @param ipos Robot's current (x, y) position
     * @param iheading Robot's current heading (converted to radians)
     */
    void Odom::setPos(QPoint ipos, QAngle iheading) {
        pos.p = {ipos.y * -1, ipos.x};
        pos.a = iheading.convert(radian) * -1;
    }

    /**
     * Returns the value of the inertial sensor for debugging
     */
    double Odom::getInternalIMU() {
        return imu1.get();
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
     * Return the internal tracking wheel position
     *
     * @return Current tracking wheel position (inches)
     */
    QLength Odom::getRightTrack() {
        return degToIn(rightEncoder.get()) * inch;
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
    void Odom::withSensors(const ADIEncoder& right, const ADIEncoder& middle, const IMU& inertial1, const IMU& inertial2, const RotationSensor& leftVel, const RotationSensor& rightVel) {
        rightEncoder = right;
        middleEncoder = middle;
        imu1 = inertial1;
        imu2 = inertial2;
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
        rightEncoder.reset();
        middleEncoder.reset();
        imu1.reset();
        startTask();
    }

    /**
     * Stops the internal thread
     */
    void Odom::stopLoop() {
        stopTask();
    }

}