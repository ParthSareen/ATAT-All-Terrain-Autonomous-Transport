// Motor Control
#include "drive.h"

//Initializing Pin
const int PWM_PIN = 11;
const int DIR_PIN = 10;
bool flag = HIGH;

// Initialize Drive with pins used for motor control 
Drive AtAt(10,11,8,9);//PWM-L,PWM-R,DIR-L,DIR-R

void setup() {
  Serial.begin(9600); 
}

void loop() {
  AtAt.accelerate(ACC_HIGH);
  
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
