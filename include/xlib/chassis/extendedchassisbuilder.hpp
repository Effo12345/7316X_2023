#pragma once
#include "xlib/chassis/extendedchassis.hpp"

namespace xlib {

    class ExtendedChassisBuilder {
        std::shared_ptr<MotorGroup> left;
        std::shared_ptr<MotorGroup> right;

        AbstractMotor::GearsetRatioPair gearset {AbstractMotor::gearset::invalid, 1.0};
        ChassisScales scales {{1, 1}, imev5GreenTPR};
        float trackWidth;

        std::shared_ptr<ADIEncoder> rightEncoder;
        std::shared_ptr<ADIEncoder> middleEncoder;
        std::shared_ptr<RotationSensor> leftVel;
        std::shared_ptr<RotationSensor> rightVel;
        std::shared_ptr<IMU> imu;

        IterativePosPIDController::Gains distanceGains;
        IterativePosPIDController::Gains turnGains;
        IterativePosPIDController::Gains angleGains;
        std::vector<float> purePursuitGains;

        float maxVelocity;
        float absoluteMaxVelocity;
        float maxAcceleration;
        float k;
        float maxRateChange;

        float lookahead;


    public:

        ExtendedChassisBuilder() = default;

        ExtendedChassisBuilder& withMotors(const MotorGroup& ileft, const MotorGroup& iright);

        ExtendedChassisBuilder& withDimensions(const AbstractMotor::GearsetRatioPair& igearset, const ChassisScales& iscales, QLength itrackWidth);

        ExtendedChassisBuilder& withMaxVelocity(const float absolMaxVel);

        ExtendedChassisBuilder& withSensors(const ADIEncoder& iright, const ADIEncoder& imiddle, 
                                           const RotationSensor ileftVelocity, const RotationSensor irightVelocity, 
                                           const IMU& inertial);

        ExtendedChassisBuilder& withGains(const IterativePosPIDController::Gains& idistanceGains, const IterativePosPIDController::Gains& iturnGains, const IterativePosPIDController::Gains& iangleGains, const IterativePosPIDController::Gains& ipurePursuitGains);

        ExtendedChassisBuilder& withVelocityConstants(float maxVel, float maxAccel, float curveConstant, float maxChange);

        ExtendedChassisBuilder& withLookahead(QLength ilookahead);

        std::shared_ptr<xlib::ExtendedChassis> build();
    };

}