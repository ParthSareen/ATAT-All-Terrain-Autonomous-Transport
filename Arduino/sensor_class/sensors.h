#ifndef sensors_h
#define sensors_h
#include "Arduino.h"
#include "HCSR04.h"
#include "MPU6050.h"

class Sensors {
  public:
    Sensors(int trig_pin, int echo_pin[3], int num_sensors);
    void readUltrasonic(HCSR04 *hc, float usReadings[3]);
    int16_t * readIMU(MPU6050 accelgyro);
    bool calibrateUltrasonic(int num_readings, HCSR04 hc);
    int * calibrateIMU(MPU6050 accelgyro);
  
  private:
    int _trig_pin;
    int _num_sensors;
    int* _echo_pin;
};

#endif