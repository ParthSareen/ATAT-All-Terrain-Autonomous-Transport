#ifndef sensors_h
#define sensors_h
#include "Arduino.h"
#include "HCSR04.h"
#include "MPU6050.h"
#include "I2Cdev.h"
#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_VL53L0X.h"

class Sensors {
public:
    explicit Sensors(int num_sensors);
    bool calibrateUltrasonic(int num_readings, HCSR04 hc);
    void calibrateICM(Adafruit_ICM20948* icm);
    void readICM(Adafruit_ICM20948* icm,  float icmReadings[6]);
    void ultrasonicSetup(int trig_pin, int echo_pin_front, int echo_pin_left);
    void readUltrasonicBetter(float us_readings[2]);
    void setupTOFs(Adafruit_VL53L0X* lox1, Adafruit_VL53L0X* lox2, int xshut1, int xshut2);
    void readTOFs(float tof_readings[2], bool debug=false);

private:

    Adafruit_VL53L0X* _lox1;
    Adafruit_VL53L0X* _lox2;
    int _trig_pin;
    int _echo_pin_front;
    int _echo_pin_left;
    int _num_sensors;
    int* _echo_pin;
    static void _powerCycle(int xshut1, int xshut2);

};

#endif