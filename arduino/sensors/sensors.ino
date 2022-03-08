// Motor Control
#include "sensors.h"

// Initialize Sensor Pins
Sensors ATAT(10,11,8,9);//Trig, Echo Array, Number of Sensors

void setup() {
  Wire.begin();
  Serial.begin(115200);
  accelgyro.initialize();

  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
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
