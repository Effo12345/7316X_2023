#include "utils.hpp"

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