// Motor Control
#include "sensors.h"
#include "MPU6050.h"
#include "Wire.h"

//Setting up Sensor 
int echo_array[3] = {8,9,10};
Sensors ATAT(7, echo_array, 3);

//Setting up Ultrasonic and IMU
HCSR04 hc(7, new int[3]{8, 9, 10}, 3);
MPU6050 accelgyro;

float *ultrasonicReadings;
int16_t *imuReadings;
int *imuCalibration;

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Ultrasonic Stuff");
}

void loop() {
  //Test for Calibration
  //bool status = ATAT.calibrateUltrasonic(5, hc);
  //Serial.println(status);

  //Test for Reading Ultrasonic
  //ultrasonicReadings = ATAT.readUltrasonic(hc);
  //for ( int i = 0; i < 3; i++ ) {
      //Serial.println(*(ultrasonicReadings + i));
   //}

   //Test for IMU Reading
   //imuReadings = ATAT.readIMU(accelgyro);
   //for ( int i = 0; i < 6; i++ ) {
   //   Serial.println(*(imuReadings + i));
   //}

   //Test for IMU Calibration
   imuCalibration = ATAT.calibrateIMU(accelgyro);
   for ( int i = 0; i < 6; i++ ) {
      Serial.println(*(imuCalibration + i));
   }
}