#include "xlib/chassis/pathfollower.hpp"
#include <cmath>
#include <stdexcept>

namespace xlib {

    /**
     * Find the index of the closest point in path to pos using the distance
     * formula.
     *
     * @param path Points to iterate through
     * @param pos Position all distances are measured from
     * @param startIndex Start the search at this index
     * @return The index of the closest point to pos
     */
    int PathFollower::findClosestPointIndex(std::vector<QPoint>& path, QPoint pos, int startIndex) {
        float lastClosestDistance = 100;
        int closestIndex = startIndex;
        for(int i = startIndex; i < path.size(); i++) {
            float distance = pos.distanceTo(path[i]);
                
            if(distance < lastClosestDistance) {
                lastClosestDistance = distance;
                closestIndex = i;
            }
        }
        return closestIndex;
    }

    /**
     * Find the point(s) along the path where a circle cast about the robot's
     * tracking center intersects the vectors of the path. fractionalIndex is
     * modified as a reference.
     *
     * @param path Path data to find intersections on
     * @param pos Center of the circle to cast
     * @param lookaheadDistance Radius of the circle about the robot
     * @param fractionalIndex How far along the path the intersection is
     * @return The intersection point furthest along the path
     */
    QPoint PathFollower::findLookaheadPoint(std::vector<QPoint>& path, QPoint pos, float lookaheadDistance, float& fractionalIndex) {
        QPoint intersection;
        float proposedFractionalIndex;
        //Starting point for the loop is defined as fractionalIndex truncated to an
        //int. This increases efficiency because the lookahead point can only ever
        //move forwards
        for(int i = static_cast<int>(fractionalIndex); i < path.size() - 1; i++) {
            QPoint d = path[i+1] - path[i];
            QPoint f = path[i] - pos;

            float a = d.dotProduct(d);
            float b = 2 * f.dotProduct(d);
            float c = f.dotProduct(f) - pow(lookaheadDistance, 2);

            float discriminant = pow(b, 2) - (4 * a * c);

            if(discriminant < 0) {
                //No intersection, continue to next iteration
                continue;
            }

            discriminant = sqrt(discriminant);

            float t1 = (-b - discriminant) / (2 * a);
            float t2 = (-b + discriminant) / (2 * a);

            //T2 intersection is always further along the path
            if(t2 >= 0 && t2 <= 1) {
                //Calculate for t2 intersection
                intersection = path[i] + d * t2;
                proposedFractionalIndex = i + t2;
            }
            else if(t1 >=0 && t1 <= 1){
                //Calculate for t1 intersection
                intersection = path[i] + d * t1;
                proposedFractionalIndex = i + t1;
            }
            else {
                continue;
            }

            if(proposedFractionalIndex > fractionalIndex) {
                fractionalIndex = proposedFractionalIndex;
                return intersection;
            }
        }
        //If no intersection is found, throw error for the caller to deal with
        throw std::runtime_error("Point unchanged from previous");
    }

    /**
     * Find the curvature of the arc that connects the current position and the
     * lookahead point (the intersection of the circle cast about the robot and
     * the path)
     *
     * @param pos The robot's current position
     * @param heading The robot's current yaw
     * @param lookahead The intersection of the lookahead circle and the path
     * @param lookaheadDistance Radius of lookahead circle
     * @return Curvature to the lookahead point
     */
    float PathFollower::findArcCurvature(QPoint pos, float heading, QPoint lookahead, float lookaheadDistance) {
        QPoint headingVector = {sin(heading), cos(heading)};
        QPoint lookaheadVector = lookahead - pos;
        float lookaheadVectorMagnitude = lookaheadVector.getMagnitude();
        
        //Find the target heading after the proposed arc movement
        float targetHeading = std::acos(std::clamp(headingVector.dotProduct(lookaheadVector) / lookaheadVectorMagnitude, -1.0f, 1.0f));

        //Find whether turn is to the left or right (negative or positive)
        //TODO: multiply by -1
        int side = sgn(((-1 * headingVector.y) * lookaheadVector.x) + (headingVector.x * lookaheadVector.y)) * -1;

        //TODO: Change this to sin
        float x = sin(targetHeading) * lookaheadVectorMagnitude;

        if(std::isnan(side * (2 * x) / pow(lookaheadDistance, 2))) {
            std::string badCurve = "Bad curvature: " + std::to_string(side * (2 * x) / pow(lookaheadDistance, 2)); // + "\nTarget: " + std::to_string(targetHeading) + "\nSide: " + std::to_string(side) + "\nLookahead: " + std::to_string(lookaheadVectorMagnitude) + "\nHeaingVector: " + headingVector.toString() + "\nLookaheadVector: " + lookaheadVector.toString() + "\nDotProduct: " + std::to_string(dotProduct) + "\nErrno: " + strerror(errno) + "\naCos: " + std::to_string(headingVector.dotProduct(lookaheadVector) / lookaheadVectorMagnitude);
            throw std::runtime_error(badCurve);
            
        }

        return side * (2 * x) / pow(lookaheadDistance, 2);
    }

    /**
     * Uses the target curvature and the robot's track width to compute the
     * left and right wheel velocities necessary to follow the desired path
     *
     * @param targetVelocity Target velocity of the robot's tracking center
     * @param curvature Curvature of the arc to follow
     * @param trackWidth Distance between the robot's wheels
     * @param isReversed Indicates if reversed pathing is desired
     */
    Odom::Velocity PathFollower::calculateWheelVelocities(float targetVelocity, float curvature, float trackWidth, bool isReversed) {
        float leftVel = targetVelocity * (2 + (curvature * trackWidth)) / 2;
        float rightVel = targetVelocity * (2 - (curvature * trackWidth)) / 2;

        if(isReversed)
            return {-rightVel, -leftVel};

        return {leftVel, rightVel};
    }

    /**
     * The heart of the pure pursuit controller, this calls helper functions to
     * find the left and right wheel velocities needed for the robot to follow a
     * pre-processed path. Uses a velocity PID controller to ensure the wheels 
     * are rotating at the correct velocity
     *
     * @param pos The robot's current position
     * @param measuredVel The velocity (rpm) of the drivetrain wheels
     * @return The desired left and right wheel velocities
     */
    Odom::Velocity PathFollower::followPath(Odom::QPos pos, Odom::Velocity measuredVel) {
        //followPath gets its own copy of pos, so no need for thread safety

        //Calculate the error from the robot's position to the end of the path to give to the settledUtil
        error = pos.p.distanceTo(path->points[path->points.size() - 1]);

        //If reverse pathing is desired, make the robot appear reversed so curvature computes correctly
        if(settings->reversed)
            pos.a += okapi::pi;

        //Find closest point on the path
        int closestPoint = findClosestPointIndex(path->points, pos.p, lastClosestPointIndex);
        lastClosestPointIndex = closestPoint;

        QPoint lookaheadPoint;
        //Find the lookahead point, catching a potential error of no new lookahead being found
        try {
            lookaheadPoint = lastLookahead = findLookaheadPoint(path->points, pos.p, settings->lookaheadDistance, lastFractionalIndex);
        } 
        catch (std::exception e) {
            //If the no new lookahead point was found, use the previous one
            lookaheadPoint = lastLookahead;
        }

        //Find the curvature of the movement arc
        float curvature = findArcCurvature(pos.p, pos.a, lookaheadPoint, settings->lookaheadDistance);

        float targetVelocity;
        //Calculate the target wheel speeds
        if(settings->useRateLimiter)
            targetVelocity = limit.constrain(path->velocity[closestPoint], settings->maxRateChange);
        else
            targetVelocity = path->velocity[closestPoint];

        Odom::Velocity targetVelocities = calculateWheelVelocities(targetVelocity, curvature, settings->trackWidth, settings->reversed);

        //Control wheel velocities using velocity PID (loop must run every 25 msec)
        Odom::Velocity feedForward {(settings->kV * targetVelocities.leftVel) + (settings->kA * ((targetVelocities.leftVel - lastVelocities.leftVel) / 0.025)),
                                    (settings->kV * targetVelocities.rightVel) + (settings->kA * ((targetVelocities.rightVel - lastVelocities.rightVel) / 0.025))};

        Odom::Velocity feedF = (targetVelocities * settings->kV) + (((targetVelocities - lastVelocities) / 0.025) * settings->kA);
        lastVelocities = targetVelocities;

        Odom::Velocity feedBack {settings->kP * (targetVelocities.leftVel - measuredVel.leftVel),
                                settings->kP * (targetVelocities.rightVel - measuredVel.rightVel)};

        Odom::Velocity feedB = (targetVelocities - measuredVel) * settings->kP;

        //grapher.newData(targetVelocities.leftVel, 0);
        //grapher.newData(measuredVel.leftVel, 1);

        std::string output = "Closest point: " + std::to_string(closestPoint) + "Top point: " + std::to_string(path->points.size());
        pros::lcd::set_text(3, output);
        std::string lookhaeadOutput = "Lookahead: (" + std::to_string(lookaheadPoint.x) + ", " + std::to_string(lookaheadPoint.y) + ")";
        pros::lcd::set_text(4, lookhaeadOutput);
        std::string curvatureOutput = "Curvature: " + std::to_string(curvature);
        pros::lcd::set_text(5, curvatureOutput);
        std::string targetVelocityOutput = "Target: " + std::to_string(targetVelocity);
        pros::lcd::set_text(6, targetVelocityOutput);
        std::string velocityOutput = "Velocity: (" + std::to_string(targetVelocities.leftVel) + ", " + std::to_string(targetVelocities.rightVel) + ")";
        pros::lcd::set_text(7, velocityOutput);

        //Divide by the absolute max velocity the drivetrain is capable of to 
        //normalize the value between [-1, 1].
        return (feedForward + feedBack) / settings->absoluteVelocityLimit;


        /*
        okapi::MotorGroup leftMotors({12, -13, 14});
        okapi::MotorGroup rightMotors({-8, 9, -10});

        //while loop starts here
        //while(true) {
            //Copy position for thread protection
            Odom::QPos position;
            position.p.x = pos.p.x;
            position.p.y = pos.p.y;
            position.a = pos.a;

            //If reverse pathing is desired,
            //make the robot appear reversed so curvature computes correctly
            if(settings->reversed) {
                position.a += 3.14159265f;
            }



            //=================================================================================
            //Find closest point on the path
            float lastClosestDistance = 100;
            for(int i = closestPoint; i < path->points.size(); i++) {
                float distance = sqrt(pow(path->points[i].x - position.p.x, 2) + pow(path->points[i].y - position.p.y, 2));

                if(distance < lastClosestDistance) {
                    lastClosestDistance = distance;
                    closestPoint = i;
                }
            }

            
            //=================================================================================
            //Find intersection (lookahead point)
            for(int i = intersectIndex; i < path->points.size() - 1; i++) {
                //E is the starting point of the vector
                //L is the end point of the vector
                //C is the center of the circle (with r lookahead distance)

                //d = L - E
                QPoint d;
                d.x = path->points[i + 1].x - path->points[i].x;
                d.y = path->points[i + 1].y - path->points[i].y;

                //f = E - C
                QPoint f;
                f.x = path->points[i].x - position.p.x;
                f.y = path->points[i].y - position.p.y;

                //a = d.DotProduct(d)
                float a = (d.x * d.x) + (d.y * d.y);
                //b = 2 * f.DotProduct(d)
                float b = 2 * ((f.x * d.x) + (f.y * d.y));
                //c = f.DotProduct(f) - r * r
                //r is the radius of the sphere, the lookahead distance
                float c = ((f.x * f.x) + (f.y * f.y)) - (lookaheadDistance * lookaheadDistance);

                //discriminant = b * b - 4 * a * c
                float discriminant = (b * b) - (4 * a * c);

                if(discriminant < 0) {
                    //No intersection
                }
                else {
                    discriminant = sqrt(discriminant);

                    float t1 = (-b - discriminant) / (2 * a);
                    float t2 = (-b + discriminant) / (2 * a);

                    //T2 intersection is always further along the path
                    if(t2 >= 0 && t2 <= 1) {
                        //return t2 intersection
                        QPoint intersection;
                        intersection.x = path->points[i].x + t2 * d.x;
                        intersection.y = path->points[i].y + t2 * d.y;

                        //Find the fractional index of the intersection and compare it to the previous lookahead point
                        float tempFractionalIndex = i + t2;
                        if(tempFractionalIndex > fractionalIndex) {
                            //If the fractional index is greater,
                            //set the new values for the next loop and the rest of the algorithm
                            fractionalIndex = tempFractionalIndex;
                            intersectIndex = i;
                            lookaheadPoint.x = intersection.x;
                            lookaheadPoint.y = intersection.y;

                            //Cancel the loop as the value has been found
                            break;
                        }
                    }
                    if(t1 >= 0 && t1 <= 1) {
                        //return t1 intersection
                        QPoint intersection;
                        intersection.x = path->points[i].x + t1 * d.x;
                        intersection.y = path->points[i].y + t1 * d.y;

                        //Find the fractional index of the intersection and compare it to the previous lookahead point
                        float tempFractionalIndex = i + t1;
                        if(tempFractionalIndex > fractionalIndex) {
                            //If the fractional index is greater,
                            //set the new values for the next loop and the rest of the algorithm
                            fractionalIndex = tempFractionalIndex;
                            intersectIndex = i;
                            lookaheadPoint.x = intersection.x;
                            lookaheadPoint.y = intersection.y;

                            //Cancel the loop as the value has been found
                            break;
                        }
                    }
                }
            }



            //=================================================================================
            //Calculate curvature of movement arc
            QPoint headingVector;
            headingVector.x = sin(position.a);
            headingVector.y = cos(position.a);

            QPoint lookaheadVector;
            lookaheadVector.x = lookaheadPoint.x - position.p.x;
            lookaheadVector.y = lookaheadPoint.y - position.p.y;
            float lookaheadVectorMagnitude = sqrt(pow(lookaheadVector.x, 2) + pow(lookaheadVector.y, 2));

            float targetHeading = acos((headingVector.x * lookaheadVector.x
                                    + headingVector.y * lookaheadVector.y)
                                    / lookaheadVectorMagnitude);

            //Find whether the turn is to the left or right (negative or positive)
            int side = sgn(((-1 * headingVector.y) * lookaheadVector.x) + (headingVector.x * lookaheadVector.y)) * -1;

            float x = sin(targetHeading) * lookaheadVectorMagnitude;

            float curvature = side * (2 * x) / pow(lookaheadDistance, 2);




            //=================================================================================
            //Compute wheel velocities
            //L = target left wheel's speed
            //R = target right wheel's speed
            //T = track width
            float T = 17; //inches

            //Rate limiter call
            //TODO: Fix rate limiter
            float targetVelocity;
            targetVelocity = path->velocity[closestPoint];


            //Target left wheel speed
            float L = targetVelocity * (2 + (curvature * T)) / 2;
            float R = targetVelocity * (2 - (curvature * T)) / 2;

            if(settings->reversed) {
                float tmp = L;

                L = -1 * R;
                R = -1 * tmp;
            }


            L /= settings->absoluteVelocityLimit;
            R /= settings->absoluteVelocityLimit;

            Odom::Velocity returnVel {L, R};
            return(returnVel);


            //=================================================================================
            //Telemtry for output data
            std::string output = "Closest point: " + std::to_string(closestPoint) + "Top point: " + std::to_string(path->points.size());
            pros::lcd::set_text(4, output);
            std::string lookAheadOutput = "Lookahead: (" + std::to_string(lookaheadPoint.x) + ", " + std::to_string(lookaheadPoint.y) + ")";
            pros::lcd::set_text(5, lookAheadOutput);
            std::string targetHeadingOut = "Target Heading: " + std::to_string(targetHeading);
            pros::lcd::set_text(7, targetHeadingOut);
            std::string curvatureOutput = "Curvature: " + std::to_string(curvature);
            pros::lcd::set_text(7, curvatureOutput);
        //}
        */
    }

        /**
        * A public function for other classes to run followPath in a loop
        */
        Odom::Velocity PathFollower::step(Odom::QPos pos, Odom::Velocity vel) {
            return followPath(pos, vel);
        }

        /**
        * Uses an okapi settled util to determine if the robot has stopped moving
        *
        * @return Whether the robot has settled from its current movement
        */
        bool PathFollower::isSettled() {
            return settled.isSettled(error);
        }

        /**
        * Define a new path to follow and sets any necessary class-scoped variables
        * 
        * @param qpath New path to follow
        * @param set New configurations to define robot movement
        * @param pos The robot's current position
        */
        void PathFollower::setNewPath(QPath* qpath, QPath::Settings* set, Odom::QPos pos) {
            //Set internal path and settings objects
            path = qpath;
            settings = set;

            //Reset variables with class scope to their default values
            //The robot's current position is used as the default lookahead point so 
            //the robot doesn't cross the center line during auton in case the algorithm
            //fails to find a lookahead point
            lastClosestPointIndex = 0.0;
            lastFractionalIndex = 0.0;
            lastLookahead = pos.p;
            lastVelocities = {0, 0};
            error = 0.0;
            limit.reset();

            //grapher.initGraph({0, 300}, 250);
        }

}