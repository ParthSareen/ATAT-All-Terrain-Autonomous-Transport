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

#define UP 0
#define RIGHT 1
#define DOWN 2 
#define LEFT 3
#define PWM_PIN_LEFT D3
#define PWM_PIN_RIGHT D6
#define DIR_PIN_LEFT D4
#define DIR_PIN_RIGHT D7
#define ENC_PIN_LEFT D5
#define ENC_PIN_RIGHT D8

#define TILE_LENGTH 30.5
Adafruit_ICM20948 icm;

HCSR04 hc(D5, ea, NUM_US); 
int echo_array[2] = {D1,D2};
Sensors ATAT(D0, echo_array, 2);
long duration;
int distance;
//Setting up Ultrasonic and IMU and motor
Drive motor_control(PWM_PIN_LEFT, PWM_PIN_RIGHT, DIR_PIN_LEFT, DIR_PIN_RIGHT); 
void setup(){

}

void loop(){ 
    delay(10000);
    motor_control.turn_right(MAX_SPEED);
    delay(5000);
    motor_control.estop();
    delay(1000000);
}