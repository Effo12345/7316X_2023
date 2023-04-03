#pragma once
#include "xlib/chassis/path.hpp"
#include "xlib/chassis/odometry.hpp"
#include "xlib/utils.hpp"
#include "xlib/display/grapher.hpp"

namespace xlib {

    class PathFollower{
        QPath* path;
        QPath::Settings* settings;

        rateLimiter limit;

        int lastClosestPointIndex = 0;
        float lastFractionalIndex = 0.0f;
        QPoint lastLookahead {0.0f, 0.0f};
        Odom::Velocity lastVelocities {0.0f, 0.0f};

        float error = 0.0f;
        SettledUtil settled{std::make_unique<Timer>(), 2, 2, 100_ms};

        Grapher grapher;

        //Generalized template function to get the sign of a number
        template <typename T> int sgn(T val) {
            return (T(0) < val) - (val < T(0));
        }

        int closestPoint = 1;
        int intersectIndex = 0;
        float fractionalIndex = 0.0f;

        QPoint lookaheadPoint;
        float lookaheadDistance = 15;
        
        int findClosestPointIndex(std::vector<QPoint>& path, QPoint pos, int startIndex = 0);

        //BE CAREFUL paramater fractionalIndex has a side effect and is modified as a reference
        QPoint findLookaheadPoint(std::vector<QPoint>& path, QPoint pos, float lookaheadDistance, float& fractionalIndex);

        float findArcCurvature(QPoint pos, float heading, QPoint lookahead, float lookaheadDistance);

        Odom::Velocity calculateWheelVelocities(float targetVelocity, float curvature, float trackWidth, bool isReversed);

        Odom::Velocity followPath(Odom::QPos pos, Odom::Velocity measuredVel);

    public:
        Odom::Velocity step(Odom::QPos pos, Odom::Velocity vel);

        bool isSettled();

        void setNewPath(QPath* qpath, QPath::Settings* set, Odom::QPos pos);
    };

}