
#include "sensors.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include "HCSR04.h"
#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"
#include "Adafruit_Sensor.h"

#define OUTPUT_READABLE_ACCELGYRO
#define ICM_CS 10
#define ICM_SCK 13
#define ICM_MISO 12
#define ICM_MOSI 11
// ============== Public Methods ==============

Sensors::Sensors(int num_sensors){
    // _trig_pin = trig_pin;
    // _echo_pin = echo_pin;
    _num_sensors = num_sensors;
}

/*
* Calibrating Ultrasonic Sensor
* Function: Reads 10 sensor readings and averages - then check to see if it fits in tolerance
* Returns: Bool of within tolerance or not
*/
bool Sensors::calibrateUltrasonic(int num_readings, HCSR04 hc){
    Serial.println("==> Calibrating Ultrasonic");
    float averages[3];
    bool status;

    // Read each sensor a certain number of times and find the average
    for (int i = 0; i < num_readings; i++){
        for (int j = 0; j < 3; j++){
            Serial.println(hc.dist(j));
            averages[j] = hc.dist(j) + averages[j];
        }
    }

    for (int i = 0; i < 3; i++){
        averages[i] = averages[i]/num_readings;
        Serial.println(averages[i]);
        delay(60);
        if ((averages[i] < 4.5) || (averages[i] > 5.5))
            return false;
        Serial.println("Calibration failed");
    }

    Serial.println("Calibration Success");
    return true;
}

void Sensors::ultrasonicSetup(int trig_pin, int echo_pin_front, int echo_pin_left){
    // TODO: Refactor
    pinMode(trig_pin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echo_pin_left, INPUT);
    pinMode(echo_pin_front, INPUT);
    _trig_pin = trig_pin;
    _echo_pin_left = echo_pin_left;
    _echo_pin_front = echo_pin_front;
}

void Sensors::readUltrasonicBetter(float usReadings[2]){
    // TODO cleanup
    // US Front
    digitalWrite(_trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig_pin, HIGH);
    delayMicroseconds(10);
    float duration_front = pulseIn(_echo_pin_front, HIGH);
    float distance_front = duration_front *0.034/2;
    // Serial.print("Front: ");
    // Serial.println(distance_front);
    usReadings[0] = distance_front;

    delay(20);

    // US Left
    digitalWrite(_trig_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(_trig_pin, HIGH);
    delayMicroseconds(10);
    float duration_left = pulseIn(_echo_pin_left, HIGH);
    float distance_left = duration_left*0.034/2;
    // Serial.print("Left: ");
    // Serial.println(distance_left);
    usReadings[1] = distance_left;
}

void Sensors::calibrateICM(Adafruit_ICM20948* icm) {
    while (!Serial)
        delay(10); // will pause Zero, Leonardo, etc until serial console opens
    //   Wire.begin(D1,D2);
    Serial.println("Adafruit ICM20948 test!");
    // Try to initialize!
    if (!icm->begin_I2C()) {
        // if (!icm.begin_SPI(ICM_CS)) {
        // if (!icm.begin_SPI(ICM_CS, ICM_SCK, ICM_MISO, ICM_MOSI)) {
        Serial.println("Failed to find ICM20948 chip");
        while (1) {
            delay(10);
        }
    }
    Serial.println("ICM20948 Found!");
    // icm.setAccelRange(ICM20948_ACCEL_RANGE_16_G);
    Serial.print("Accelerometer range set to: ");
    switch (icm->getAccelRange()) {
        case ICM20948_ACCEL_RANGE_2_G:
            Serial.println("+-2G");
            break;
        case ICM20948_ACCEL_RANGE_4_G:
            Serial.println("+-4G");
            break;
        case ICM20948_ACCEL_RANGE_8_G:
            Serial.println("+-8G");
            break;
        case ICM20948_ACCEL_RANGE_16_G:
            Serial.println("+-16G");
            break;
    }
    Serial.println("OK");
    // icm.setGyroRange(ICM20948_GYRO_RANGE_2000_DPS);
    Serial.print("Gyro range set to: ");
    switch (icm->getGyroRange()) {
        case ICM20948_GYRO_RANGE_250_DPS:
            Serial.println("250 degrees/s");
            break;
        case ICM20948_GYRO_RANGE_500_DPS:
            Serial.println("500 degrees/s");
            break;
        case ICM20948_GYRO_RANGE_1000_DPS:
            Serial.println("1000 degrees/s");
            break;
        case ICM20948_GYRO_RANGE_2000_DPS:
            Serial.println("2000 degrees/s");
            break;
    }
    //  icm.setAccelRateDivisor(4095);
    uint16_t accel_divisor = icm->getAccelRateDivisor();
    float accel_rate = 1125 / (1.0 + accel_divisor);
    Serial.print("Accelerometer data rate divisor set to: ");
    Serial.println(accel_divisor);
    Serial.print("Accelerometer data rate (Hz) is approximately: ");
    Serial.println(accel_rate);
}

void Sensors::readICM(Adafruit_ICM20948* icm, float icmReadings[6]){
    Serial.println("==> Reading IMU Sensor");
    sensors_event_t accel;
    sensors_event_t gyro;
    sensors_event_t temp;
    sensors_event_t mag;
    icm->getEvent(&accel, &gyro, &temp, &mag);
    icmReadings[0] = accel.acceleration.x;
    icmReadings[1] = accel.acceleration.y;
    icmReadings[2] = accel.acceleration.z;
    icmReadings[3] = gyro.gyro.x;
    icmReadings[4] = gyro.gyro.y;
    icmReadings[5] = gyro.gyro.z;
    return;
}