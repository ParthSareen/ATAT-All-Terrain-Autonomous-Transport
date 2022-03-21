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
#define FRONT_US_DIST 7.63
#define TILE_LENGTH 30.5
Adafruit_ICM20948 icm;

HCSR04 hc(D5, ea, NUM_US); 
int echo_array[2] = {D1,D2};
Sensors ATAT(D0, echo_array, 2);
long duration;
int distance;
//Setting up Ultrasonic and IMU and motor
Drive motor_control(PWM_PIN_LEFT, PWM_PIN_RIGHT, DIR_PIN_LEFT, DIR_PIN_RIGHT); 

int changeOrientationLeft = 0; 
int changeOrientationUp = 0; 
int changeOrientationRight = 0; 
int changeOrientationDown = 0;

int orientation = LEFT; 
int track[6][6] = {  {0, 0, 0, 0, 0, 0}, 
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 1, -1, -1}
                   };

int distanceTrack[6][6] = {  {0, 0, 0, 0, 0, 0}, 
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {7.63, 38.13, 68.63, 99.13, -1, -1}
                   };

float leftArray = { 99.13, 68.63, 38.13, 7.63, 180}
//start pos is [5][3]; 
//Left or right implies y stays same 
//up or down implies other var stays same 
//use this var to keep track of current pos and update 
int current_position[2] = {5, 3}; 
float us_array[NUM_US] = {0, 0};
float imu_array[NUM_SENS] = {0, 0, 0, 0, 0, 0};

bool testState = 0; 

float ultrasonicAverageFront = 0; 
float ultrasonicAverageLeft = 0; 

float lastUltrasonicAverageFront = 0; 
float lastUltrasonicAverageLeft = 0; 


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
}

void loop() { 
  float * ultrasonicReadings = new float[NUM_US]; 
  //float * imuReadings = new float[NUM_SENS];
  //take 10 readings find average 
//  for(int i = 0; i < 10; i++){ 
//    //TODO: Read ultrasonic method will change 
//    ATAT.readUltrasonic(&hc, ultrasonicReadings); 
//    ultrasonicAverageFront += ultrasonicReadings[0]/10.00; 
//    ultrasonicAverageLeft += ultrasonicReadings[1]/10.00; 
//  }

  //This is 40ms read 
  ATAT.readUltrasonicBetter(ultrasonicReadings); 
  ultrasonicAverageFront = ultrasonicReadings[0]; 
  ultrasonicAverageLeft = ultrasonicReadings[1]; 
  
  
  if(orientation == LEFT){  
    //index check 
    if(current_position[1]-1 >= 0){ 
      //Check if current pos is visited, check if next pos in orientation is unvisited, check if ultrasonics are reading greater than half a tile 
      if(track[current_position[0]][current_position[1]] == 1 && track[current_position[0]][current_position[1]-1] == 0 && (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2)+changeOrientationLeft*TILE_LENGTH))){
         motor_control.cruise(HALF_SPEED, 1); 
         //track[current_position[0]][current_position[1]-1] = 1; 
      }
    } else { 
      motor_control.estop();
      orientation = UP; 
      changeOrientationLeft++;
      //TODO: Test speed + turning threshold 
      motor_control.turn_right(HALF_SPEED);
      while(ultrasonicAverageFront < 83){ 
        ATAT.readUltrasonicBetter(ultrasonicReadings); 
        ultrasonicAverageFront = ultrasonicReadings[0]; 
        ultrasonicAverageLeft = ultrasonicReadings[1]; 
      }
      motor_control.estop();
      int y = current_position[0];
      for(int i = current_position[1]; i >= 0; i--){ 
        if(track[y][i] == 1){ 
          current_position[0] = y; 
          current_position[1] = i+1; 
          break;
        }
        else { 
          track[y][i] = 1; 
        }
      }
    }
  }
  else if(orientation == UP){ 
    if(current_position[0]-1 >= 0){ 
      if(track[current_position[0]][current_position[1]] == 1 && track[current_position[0]-1][current_position[1]] == 0 &&  (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2)+changeOrientationUp*TILE_LENGTH))){
        motor_control.cruise(HALF_SPEED, 1);
      }
    } else { 
      motor_control.estop();
      orientation = RIGHT; 
      changeOrientationUp++;
      //TODO: Test speed + turning threshold 
      motor_control.turn_right(HALF_SPEED);
      while(ultrasonicAverageFront < 83){ 
        ATAT.readUltrasonicBetter(ultrasonicReadings); 
        ultrasonicAverageFront = ultrasonicReadings[0]; 
        ultrasonicAverageLeft = ultrasonicReadings[1]; 
      }
      motor_control.estop();
      int x = current_position[1];
      for(int i = current_position[0]; i >= 0; i--){ 
        if(track[y][i] == 1){ 
          current_position[0] = y; 
          current_position[1] = i+1; 
          break;
        }
        else { 
          track[y][i] = 1; 
        }
      }
    }
  }
  else if(orientation == RIGHT){ 
    
  }
  else if(orientation == DOWN){ 
    
  }
  //ATAT.readUltrasonic(&hc, ultrasonicReadings); 
//  ATAT.readUltrasonic(&hc, ultrasonicReadings); 
//  for(int i = 0; i < NUM_US; i++){ 
//   Serial.println(ultrasonicReadings[i]); 
//  }
  Serial.println("Moving foward"); 
  moveForward(); 
  delay(4000); 
  Serial.println("Done function"); 

  //ATAT.readICM(&icm, imuReadings);
  for(int i = 0; i < NUM_SENS; i++){ 
    Serial.println(imuReadings[i]);
  }
  ultrasonicReadings = NULL;
  imuReadings = NULL;
  //delete[] imuReadings;
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
