#ifndef drive_h
#define drive_h
#include "Arduino.h"

// Motor constants
const int MAX_RPM = 159;
const float MAX_SPEED = 0.333; 

enum drive_mode {
    ACCELERATING = 0, 
    DECELERATING = 1, 
    TURNING = 2, 
    STOPPED = 3, 
    CRUISING = 4, 
    REVERSING = 5
};

enum accel_rate {
  ACC_ZERO = 0,
  ACC_MINIMAL = 1,
  ACC_LOW = 2,
  ACC_MEDIUM = 3,
  ACC_HIGH = 4,
  ACC_IMMENSE = 5,
  ACC_MAXIMUM = 6
};

enum motor_spin {
    COUNTERCLOCKWISE = 0,
    CLOCKWISE = 1
}

class Drive {
  public:
    Drive(int pwm_pin_left, int pwm_pin_right, int dir_pin_left, int dir_pin_right);
    void accelerate(int rate);
    void decelerate(int rate);
    void estop();
    void turn_right();
    void turn_left();
    void reverse();
    void cruise();
  
  private:
    int _pwm_pin_left;
    int _pwm_pin_right;
    int _dir_pin_left;
    int _dir_pin_right;
    int _current_speed; 
    int _mode; 
};

#endif
