#include "sensors.h"
#include "Arduino.h"
#include "sensors.h"
#include "MPU6050.h"
#include "HCSR04.h"
#include "Wire.h"
#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"
#include "Adafruit_Sensor.h"
#define NUM_US 2
#define NUM_SENS 6
//#define trig_pin 7
Adafruit_ICM20948 icm;

int *ea = new int[NUM_US]{D6, D7}; 
HCSR04 hc(D5, ea, NUM_US); 
int echo_array[3] = {8,9,10};
Sensors ATAT(7, echo_array, 2);
long duration;
int distance;
//Setting up Ultrasonic and IMU

int track[6][6] = {  {0, 0, 0, 0, 0, 0}, 
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 1, -1, -1}
                   };

float us_array[NUM_US] = {0, 0};
float imu_array[NUM_SENS] = {0, 0, 0, 0, 0, 0};



bool testState = 0; 

void getTelemetryData(){ 
  Serial.println("getting telemetry data");
}

void sendTelemetryData(){ 
  Serial.println("moving forward");
}

void forward(int speed){ 
  Serial.println("moving forward");
}

void turn(){ 
  Serial.println("turned 90 degrees"); 
}

void ultrasonicRead(){ 
  Serial.println("Round robin reads from US"); 
}

void imuRead(){ 
  Serial.println("Reading from IMU"); 
}

//void customUS(int trig_pin, int echo_pin, int i) {
////    pinMode(trig_pin, OUTPUT);
////    pinMode(echo_pin, INPUT);
//  digitalWrite(trig_pin, LOW);
//  delayMicroseconds(2);
//  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
//  digitalWrite(trig_pin, HIGH);
//  delayMicroseconds(10);
//  digitalWrite(trig_pin, LOW);
//  // Reads the echoPin, returns the sound wave travel time in microseconds
//  duration = pulseIn(echo_pin, HIGH);
//  // Calculating the distance
//  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
//  // Displays the distance on the Serial Monitor
//  Serial.print("i: ");
//  Serial.print(i);
//  Serial.print(" Distance: ");
//  Serial.print(distance);
//  Serial.println(" cm");
//  
//}

bool sensorDiagnostic(){ 
//  us_array = ultrasonicRead(); 
//  imu_array = imuRead(); 
//
//  for(int i = 0; i < us_array.size(); i++){ 
//    if(!us_array[i]){ 
//      return false; 
//    }
//  }
//  for(int i = 0; i < imu_array.size(); i++){ 
//    if(!imu_array[i]){ 
//      return false; 
//    }
//  }
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  // D6, D7, D8
//  pinMode(D6, INPUT);
//  pinMode(D7, INPUT);
//  pinMode(D8, INPUT);
//  pinMode(D5, OUTPUT);

//  pinMode(trig_pin, OUTPUT); // Sets the trigPin as an OUTPUT
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");

  Serial.println("Calibrating ICM");
  ATAT.calibrateUltrasonic(6, &hc); 
  //ATAT.calibrateICM(&icm);
  Serial.println("Ultrasonic Stuff");
  //Pin setup 
//  pinMode(resetPin, INPUT);
//  pinMode(switchPin, INPUT);
//  bool improperShutdown = 0;
//  bool sensorCheck = sensorDiagnostic();
//    if(sensorCheck == false){
//    improperShutdown = 1; 
//    uploadData(improperShutdown); 
//    exit(0)
//  }
//  improperShutdown = lastShutdownValue();
//  switchState = digitalRead(switchPin);
  
}

void loop() { 
//  bool status = ATAT.calibrateUltrasonic(5, hc);
  float * ultrasonicReadings = new float[NUM_US]; 
  float * imuReadings = new float[NUM_SENS];
   ATAT.readUltrasonic(&hc, ultrasonicReadings); 
   for(int i = 0; i < NUM_US; i++){ 
     Serial.println(ultrasonicReadings[i]); 
  }


//  ATAT.readICM(&icm, imuReadings);
//  for(int i = 0; i < NUM_SENS; i++){ 
//    Serial.println(imuReadings[i]);
//  }
  ultrasonicReadings = NULL;
  imuReadings = NULL;
  delete[] imuReadings;
  delete[] ultrasonicReadings; 
  delay(1000);
  //Serial.println(status);
  
}
