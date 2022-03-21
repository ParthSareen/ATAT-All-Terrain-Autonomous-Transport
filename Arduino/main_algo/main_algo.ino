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

Adafruit_ICM20948 icm;

int *ea = new int[NUM_US]{D1, D2}; 
HCSR04 hc(D5, ea, NUM_US); 
int echo_array[2] = {D1,D2};
Sensors ATAT(D0, echo_array, 2);
long duration;
int distance;
//Setting up Ultrasonic and IMU and motor
Drive motor_control(PWM_PIN_LEFT, PWM_PIN_RIGHT, DIR_PIN_LEFT, DIR_PIN_RIGHT); 

float ultrasonicArrayVals[10] = {0};

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

void moveForward(){ 
  motor_control.cruise(MAX_SPEED, true); 
}

void test(float ultrasonicReadings[3]){ 
  Serial.println("Testing moving forward ===>"); 
  motor_control.cruise(MAX_SPEED, true); 
  if(ultrasonicReadings[0] < 50){ 
    motor_control.deccelerate(1, true); 
    motor_control.estop(); 
  }
  delay(2000); 
  motor_control.turn_right(1); 
  delay(2000);
  motor_control.accelerate(1, true); 
  if(ultrasonicReadings[0] < 50){ 
    motor_control.deccelerate(1, true); 
    motor_control.estop(); 
  }
  motor_control.estop(); 
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
  //ATAT.calibrateUltrasonic(6, &hc); 
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
  delay(2000);
//  bool status = ATAT.calibrateUltrasonic(5, hc);
  float * ultrasonicReadings = new float[NUM_US]; 
  float * imuReadings = new float[NUM_SENS];
//  ATAT.readUltrasonic(&hc, ultrasonicReadings); 
//  for(int i = 0; i < NUM_US; i++){ 
//   Serial.println(ultrasonicReadings[i]); 
//  }
  Serial.println("Moving foward"); 
  moveForward(); 
  delay(4000); 
  Serial.println("Done function"); 

  ATAT.readICM(&icm, imuReadings);
  for(int i = 0; i < NUM_SENS; i++){ 
    Serial.println(imuReadings[i]);
  }
  ultrasonicReadings = NULL;
  imuReadings = NULL;
  delete[] imuReadings;
  delete[] ultrasonicReadings; 
  delay(1000);
  motor_control.estop(); 
  while(1){ 
    ATAT.readICM(&icm, imuReadings);
    for(int i = 0; i < NUM_SENS; i++){ 
      Serial.println(imuReadings[i]);
    }
//    ATAT.readUltrasonic(&hc, ultrasonicReadings); 
//    Serial.println("In while loop"); 
//    for(int i = 0; i < NUM_US; i++){ 
//     Serial.println(ultrasonicReadings[i]); 
//    }
    delay(3000); 
  }
  //Serial.println(status);
  
}
