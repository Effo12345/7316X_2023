#pragma once
#include "okapi/api.hpp"
#include "xlib/chassis/point.hpp"

namespace xlib {

    struct QPath {
        //Path data
        std::vector<QPoint> points;
        std::vector<float> curvature;
        std::vector<float> velocity;

        //Initialize a new path using a vector of points
        QPath(const std::vector<QPoint>& pointList) {
            points = pointList;
        }

        //Explicitly declared default constructor
        QPath() {};

        //Settings struct to hold various pure pursuit tuning constants
        struct Settings {
            //Path generating
            float pointSpacing = 6;
            float weight_smooth = 0.865;
            float tolerance = 0.001;
            float maxVelocity;
            float maxAcceleration;
            float absoluteVelocityLimit;
            float k;

            //Path following
            float lookaheadDistance;
            float maxRateChange;
            float gearRatio;
            float trackWidth;
            bool reversed = false;
            bool useRateLimiter = true;
            int timeLimit = 100000;

            //Tuning constants
            float kV;
            float kA;
            float kP;

            //Initialize tunings constants using an initializer list
            Settings(
                float maxVel, float absolVelLim, float maxAccel, float curveConstant, 
                float lookahead, float maxChange, float track, std::vector<float>& purePursuitGains
            ) : maxVelocity{maxVel}, absoluteVelocityLimit{absolVelLim}, maxAcceleration{maxAccel}, k{curveConstant},
                lookaheadDistance{lookahead}, maxRateChange{maxChange}, trackWidth{track},
                kV{purePursuitGains[0]}, kA{purePursuitGains[1]}, kP{purePursuitGains[2]} {}

            //Explicitly declared default constructor
            Settings() {}
        };
    };

}