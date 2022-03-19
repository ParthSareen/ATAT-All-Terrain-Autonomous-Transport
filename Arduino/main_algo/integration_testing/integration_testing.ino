#include "sensors.h"
#include "Arduino.h"
#include "MPU6050.h"
#include "HCSR04.h"
#include "Wire.h"
#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"
#include "Adafruit_Sensor.h"
#include "drive.h" 
#define NUM_US 2
#define NUM_SENS 6


#define PWM_PIN_LEFT D3
#define PWM_PIN_RIGHT D6
#define DIR_PIN_LEFT D4
#define DIR_PIN_RIGHT D7
#define ENC_PIN_LEFT D5
#define ENC_PIN_RIGHT D8

Drive motor_control(PWM_PIN_LEFT, PWM_PIN_RIGHT, DIR_PIN_LEFT, DIR_PIN_RIGHT);
void setup() {
  Serial.begin(9600); 
  // put your setup code here, to run once:

}

void loop() {
  float curr_speed = 0.0;
  int curr_rpm = 0;
  int modeMotor = 0;
  // put your main code here, to run repeatedly:
  motor_control.accelerate(ACC_MINIMAL, true);
  motor_control.get_params(&curr_speed, &curr_rpm, &modeMotor);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(modeMotor);
  delay(5000);
  motor_control.estop(); 
  delay(2000); 
  motor_control.accelerate(ACC_MINIMAL, false);
  motor_control.get_params(&curr_speed, &curr_rpm, &modeMotor);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(modeMotor);
  motor_control.estop(); 
  motor_control.turn_right(MAX_SPEED); 
  delay(10000); 
  motor_control.estop(); 
  motor_control.turn_left(MAX_SPEED); 
  delay(10000); 
  motor_control.estop(); 
  delay(10000);

}
