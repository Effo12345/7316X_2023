#include "xlib/chassis/odometry.hpp"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QLength.hpp"
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
            //Find total distance traveled by the tracking wheel
            distance = degToIn(tracking.get());
            //Calculate the distance traveled since the last update
            deltaDistance = distance - previousDistance;
            //Set the previous distance to the current distance
            previousDistance = distance;
            //Find the change in heading
            heading = imu.get() + headingOffset;
            deltaHeading = heading - previousHeading;
            previousHeading = heading;

            //Uses the length of arc moved and change in heading to find radius of arc
            arcRadius = (deltaDistance / deltaHeading);

            //Calculates the chord traveled
            chordLength = 2 * (arcRadius * (deltaHeading / 2));

            //Calculates x and y components of chordLength
            offset.x = sin(degToRad(heading - (deltaHeading / 2))) * chordLength;
            offset.y = cos(degToRad(heading - (deltaHeading / 2))) * chordLength;

        
            if(isnanf(offset.x) || isnanf(offset.y)) {
                printf("NAN in odom \n");
            }
            else {
                posThreadSafety.take();
                pos.p += offset;
                pos.a += deltaHeading;
                posThreadSafety.give();

                std::string output1 = "X: " + std::to_string(pos.p.x);
                std::string output2 = "Y: " + std::to_string(pos.p.y);
                std::string output3 = "A: " + std::to_string(heading);
                pros::lcd::set_text(1, output1);
                pros::lcd::set_text(2, output2);
                pros::lcd::set_text(3, output3);
            }

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
        //pos.p = {ipos.y, ipos.x * -1};
        pos.p = ipos;
        headingOffset = previousHeading = pos.a = iheading.convert(degree);
    }

    /**
     * Returns the value of the inertial sensor for debugging
     */
    double Odom::getInternalIMU() {
        return imu.get();
    }

    /**
     * Return the internal tracking wheel position
     *
     * @return Current tracking wheel position (inches)
     */
    QLength Odom::getTrack() {
        return degToIn(tracking.get()) * inch;
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
    void Odom::withSensors(const ADIEncoder& itracking, const IMU& inertial) {
        tracking = itracking;
        imu = inertial;
    }

    /**
     * Initializes distance constants used in odom math
     *
     * @param iscales Dimensions of tracking wheel placement and size
     */
    void Odom::setScales(QLength iscales) {
        wheelRadius = iscales.convert(inch) / 2;
    }

    /**
     * Starts the internal thread (calls loop asynchronously)
     */
    void Odom::startLoop() {
        tracking.reset();
        imu.reset();
        startTask();
    }

    /**
     * Stops the internal thread
     */
    void Odom::stopLoop() {
        stopTask();
    }

}