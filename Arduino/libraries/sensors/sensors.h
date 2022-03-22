#ifndef sensors_h
#define sensors_h
#include "Arduino.h"
#include "HCSR04.h"
#include "MPU6050.h"
#include "I2Cdev.h"
#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"
#include "Adafruit_Sensor.h"
#include "DFRobot_I2CMultiplexer.h"
#include "Adafruit_VL53L0X.h"

class Sensors {
public:
    Sensors(int num_sensors);
    bool calibrateUltrasonic(int num_readings, HCSR04 hc);
    void calibrateICM(Adafruit_ICM20948* icm);
    void readICM(Adafruit_ICM20948* icm,  float icmReadings[6]);
    void ultrasonicSetup(int trig_pin, int echo_pin_front, int echo_pin_left);
    void readUltrasonicBetter(float us_readings[2]);
    void setupTOFs(DFRobot_I2CMultiplexer* i2c_switcher, Adafruit_VL53L0X* lox);
    void readTOFs(float tof_readings[2], bool debug=false);

private:

    DFRobot_I2CMultiplexer* _i2c_switcher;
    Adafruit_VL53L0X* _lox;
    int _trig_pin;
    int _echo_pin_front;
    int _echo_pin_left;
    int _num_sensors;
    int* _echo_pin;

};

#endif