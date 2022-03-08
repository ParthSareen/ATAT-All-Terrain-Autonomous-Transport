#ifndef sensors_h
#define sensors_h
#include "Arduino.h"

class Sensors {
  public:
    Sensors(int trig_pin, int echo_pin[], int num_sensors);
    float * readUltrasonic();
    int16_t * readIMU();
    bool calibrateUltrasonic();
    int16_t * calibrateIMU();
  
  private:
    int trig_pin;
    int echo_pin[];
};

#endif
