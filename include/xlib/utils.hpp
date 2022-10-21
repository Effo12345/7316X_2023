#pragma once
#include "pros/rtos.hpp"

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

//Class to hold any necessary timing values
class timer {
  int prevTime = pros::millis();
  int time_abs;
  int time_relative;
public:
  int time();
  void reset();
};

//Rate limiter used in pure pursuit algorithm
class rateLimiter {
  timer clock;        //Timer to get time since last call
  float output = 0.0;      //Output value (must persist between function calls)
public:
  float constrain(float input, float maxRateChange);
};