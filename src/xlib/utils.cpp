#include "utils.hpp"
#include "okapi/api.hpp"

int timer::time() {
    time_abs = pros::millis();
    time_relative = time_abs - prevTime;
    return time_relative;
}

void timer::reset() {
    prevTime = pros::millis();
}



float rateLimiter::constrain(float input, float maxRateChange) {
    float maxChange = (0.025) * maxRateChange;
    float temp = input - output;

    temp = (temp < -maxChange) ? -maxChange : (temp > maxChange) ? maxChange : temp;

    clock.reset();
    output += temp;
    return output;
}

void rateLimiter::reset() {
    output = 0.0;
    clock.reset();
}

double rescale180(double angle) {
    return angle - 360.0 * std::floor((angle + 180.0) * (1.0 / 360.0));
}

okapi::QAngle rescale180(okapi::QAngle angle) {
    return rescale180(angle.convert(okapi::degree)) * okapi::degree;
}

double tickToIn(double tick, okapi::ChassisScales scale, okapi::AbstractMotor::GearsetRatioPair gearset) {
    return (tick/scale.tpr)/(gearset.ratio) * (scale.wheelDiameter.convert(okapi::inch) * okapi::pi);
}

double inToTick(double in, okapi::ChassisScales scale, okapi::AbstractMotor::GearsetRatioPair gearset) {
    return in / (scale.wheelDiameter.convert(okapi::inch) * okapi::pi) * (gearset.ratio) * scale.tpr;
}