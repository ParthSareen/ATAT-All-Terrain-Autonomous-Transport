// PWM Control
#include "drive.h"

//Initializing Pin
const int PWM_PIN = 11;
const int DIR_PIN = 10;
bool flag = HIGH;
Drive AtAt(10,11,8,9);//PWM-L,PWM-R,DIR-L,DIR-R

enum rate{
  zero,
  lower,
  low,
  medium,
  high,
  higher,
  highest
};


Drive::Drive(int pwm_pin_left, int pwm_pin_right, int dir_pin_left, int dir_pin_right){
  pinMode(pwm_pin_left, OUTPUT);
  pinMode(pwm_pin_right, OUTPUT);
  pinMode(dir_pin_left, OUTPUT);
  pinMode(dir_pin_right, OUTPUT);
  _pwm_pin_left = pwm_pin_left;
  _pwm_pin_right = pwm_pin_right;
  _dir_pin_left = dir_pin_left;
  _dir_pin_right = dir_pin_right;
}

void Drive::accelerate(int rate){ //input parameter from rate enum, e.g. "low"
  digitalWrite(_dir_pin_left, 1);
  digitalWrite(_dir_pin_right, 1);
  
  for(int i=255; i>0; i--){
    analogWrite(_pwm_pin_left, i);
    analogWrite(_pwm_pin_right, i);
    Serial.print("Speed up: ");
    Serial.println(i);
    delay(1+50*(1-rate/7)); //vary scalar multiple to adjust minimum acceleration
  }
}


void setup() {
  Serial.begin(9600); 
}

void loop() {
  AtAt.accelerate(high);
  
//  int i=0;
//
//  // Alternate direction
//  flag = flag ^ HIGH;
//  digitalWrite(DIR_PIN, flag);
//
//   //Speed up
//  for(i=255; i>0; i--){
//    analogWrite(PWM_PIN, i);
//    Serial.print("Speed up: ");
//    Serial.println(i);
//    delay(10);
//  }
//  
//  delay(250);
//    
//  //Slowdown
//  for(i=1; i<255; i++){
//    analogWrite(PWM_PIN, i);
//    Serial.print("Speed down: ");
//    Serial.println(i);
//    delay(10);
//  }

}
