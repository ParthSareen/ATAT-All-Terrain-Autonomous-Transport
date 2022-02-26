#ifndef drive_h
#define drive_h
#include "Arduino.h"

class Drive {
  public:
    Drive(int pwm_pin_left, int pwm_pin_right, int dir_pin_left, int dir_pin_right);
    void accelerate();
    void decelerate();
    void stop();
    void turn_right();
  private:
    int _pwm_pin_left;
    int _pwm_pin_right;
    int _dir_pin_left;
    int _dir_pin_right;
};

#endif