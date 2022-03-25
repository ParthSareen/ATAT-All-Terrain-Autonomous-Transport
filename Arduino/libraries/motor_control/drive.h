#ifndef drive_h
#define drive_h
#include "Arduino.h"

// Motor constants
const int MAX_RPM = 159;
const float MAX_SPEED = 0.333;

// Legacy, leaving so stuff doesn't break but should start 
// using MEDIUM_SPEED because that is actually around half speed
const float HALF_SPEED = 0.15; 

// Motor Speeds according to speed curve
// x = input for duty cycle between 0 - 255
// rpm = -0.5795e^(0.02228x) + 156
const float HIGH_SPEED = 0.137;
const float MEDIUM_SPEED = 0.072; 
const float LOW_SPEED = 0.033; 
const float NO_SPEED = 0.0; 

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
};

class Drive {
  public:
    Drive(int pwm_pin_left, int pwm_pin_right, int dir_pin_left, int dir_pin_right);
    void accelerate(int rate, bool fwd);
    void deccelerate(int rate, bool fwd);
    void estop();
    void turn_right(float speed);
    void turn_left(float speed);
    void reverse();
    void cruise(float lspeed, float rspeed, bool fwd);
    void cruise(float speed, bool fwd);
    void get_params(float* curr_speed, unsigned int* curr_rpm, int* mode);
  
  private:
    int _pwm_pin_left;
    int _pwm_pin_right;
    int _dir_pin_left;
    int _dir_pin_right;
    float _current_speed; 
    unsigned int _current_rpm;
    int _mode; 

    // Helper functions
    void _set_forward();
    void _set_backward();
    void _set_right();
    void _set_left();
};

float convert_i_to_speed(unsigned int i);
unsigned int convert_i_to_rpm(unsigned int i);
unsigned int convert_speed_to_i(float speed);
unsigned int convert_speed_to_rpm(float speed);
unsigned int convert_rpm_to_i(unsigned int rpm);

#endif
