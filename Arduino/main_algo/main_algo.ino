#include "sensors.h"
#include "Arduino.h"
#include "MPU6050.h"
#include "HCSR04.h"
#include "Wire.h"
#include "Adafruit_ICM20X.h"
#include "Adafruit_ICM20948.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_VL53L0X.h"
#include "drive.h" 

#include <telemetry.pb.h>
#include <ESP8266WiFi.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Telemetry.h>

#define NUM_TOF 2
#define NUM_SENS 6

#define UP 0
#define RIGHT 1
#define DOWN 2 
#define LEFT 3
#define XSHUT1 D0
#define XSHUT2 D7
#define PWM_PIN_LEFT D3
#define PWM_PIN_RIGHT D5
#define DIR_PIN_LEFT D4
#define DIR_PIN_RIGHT D6
#define FRONT_US_DIST 10.00
#define TILE_LENGTH 34 

// Measured
//#define CORRECTION_THRESHOLD_UPPER 21 
//#define CORRECTION_THRESHOLD_LOWER 8.5
#define CORRECTION_THRESHOLD_UPPER 16 
#define CORRECTION_THRESHOLD_LOWER 13 
Adafruit_ICM20948 icm;

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

//char ssid[] = "TP-LINK_2.4GHz_677647";
//char pass[] = "98238316";  
//char* addr = "192.168.0.100 ";
//uint16_t port  = 10101;
//
//WiFiClient  client;
//
//Telemetry telemetry(1000, &client, addr, port);
int count = 0;
Sensors ATAT(2);
long duration;
int distance;
//Setting up Ultrasonic and IMU and motor
Drive motorControl(PWM_PIN_LEFT, PWM_PIN_RIGHT, DIR_PIN_LEFT, DIR_PIN_RIGHT); 

float changeOrientationLeft = 0; 
int changeOrientationUp = 0; 
int changeOrientationRight = 0; 
int changeOrientationDown = 0;

int orientation = LEFT;
int prevOrientation = DOWN; 
  
int track[6][6] = {  {0, 0, 0, 0, 0, 0}, 
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, -1}
                   };
/**
 * {                 {0, 0, 0, 0, 0, 0}, 
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, 0},
                     {0, 0, 0, 0, 0, -1}
                   };
 */
//start pos is [5][4]; 
//Left or right implies y stays same 
//up or down implies other var stays same 
//use this var to keep track of current pos and update 
/**
 * int track[6][6]   {1, 1, 1, 1, 1, 1}, 
                     {1, 0, 0, 0, 0, 1},
                     {1, 0, 1, 0, 0, 1},
                     {1, 0, 0, 0, 0, 1},
                     {1, 0, 0, 0, 0, 1},
                     {1, 1, 1, 1, 1, -1}
                   };
 */
int currentPosition[2] = {5, 4}; 

bool pit = false; 
bool pit2 = false; 
bool testState = 0; 

float ultrasonicAverageFront = 0; 
float ultrasonicAverageLeft = 0; 

float lastUltrasonicAverageFront = 0; 
float lastUltrasonicAverageLeft = 0;

void autoCorrect(float ultrasonicAverageLeft, int changeOrientation, int delayTime) {
    if ((ultrasonicAverageLeft > (CORRECTION_THRESHOLD_UPPER+TILE_LENGTH*changeOrientation))&& (ultrasonicAverageLeft != -0.2)){
        motorControl.cruise(MAX_SPEED, HIGH_SPEED, 1);
        delay(delayTime);
//            motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
    }

    else if ((ultrasonicAverageLeft < CORRECTION_THRESHOLD_LOWER+TILE_LENGTH*changeOrientation)&& (ultrasonicAverageLeft != -0.2)){
        motorControl.cruise(HIGH_SPEED, MAX_SPEED, 1);
        delay(delayTime);
//              motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
    }

    else {
        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
    }
}

void delayWithCorrection(float ultrasonicAverageLeft, int changeOrientation, int delayTime, int whileDelay){
    int startTime = millis();

    while ((millis()-startTime) > whileDelay) {
        autoCorrect(ultrasonicAverageLeft, changeOrientation, delayTime);
    }
}

void setup() {
  Serial.begin(115200);
  //Serial.println();
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  Wire.begin();

   //WIFI
//   Serial.print("Setting up WIFI for SSID ");
//   Serial.println(ssid);
//  
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, pass);
//  
//   while (WiFi.status() != WL_CONNECTED) {
//     Serial.println("WIFI connection failed, reconnecting...");
//     delay(500);
//   }
//  
//   Serial.println("");
//   Serial.print("WiFi connected, ");
//   Serial.print("IP address: ");
//   Serial.println(WiFi.localIP());

  //TODO: readd setup tofs when work !! 
  ATAT.setupTOFs(&lox1, &lox2, XSHUT1, XSHUT2); 
  Serial.println("Calibrating ICM");
  ATAT.calibrateICM(&icm);
  delay(5000);
  
  //delay(5000);
}

void loop() {
  
  float * tofReadings = new float[NUM_TOF]; 
  float * icmReadings = new float [NUM_SENS]; 
  ATAT.readTOFs(tofReadings, false);


  ultrasonicAverageFront = tofReadings[0]/10.0; 
  ultrasonicAverageLeft = tofReadings[1]/10.0; 
  ATAT.readICM(&icm, icmReadings);

 //Telemetry 
//  if (count == 3){
//    client.connect(addr, port); 
//    telemetry.uploadMainData(tofReadings, icmReadings, orientation);
//    count = 0;
//  }
//   else {
//    count++;   
//  }
  //check if it is in a pit, if it is ignore readings. the variable Gy corresponds to pit readings.  

  

  if(icmReadings[4] < 0.2 && icmReadings[4] > -0.2){
    if(orientation == LEFT){  
      ATAT.readTOFs(tofReadings, false);
      ultrasonicAverageFront = tofReadings[0]/10.0; 
      ultrasonicAverageLeft = tofReadings[1]/10.0; 
      //index check 
        //Check if current pos is visited, check if next pos in orientation is unvisited, check if ultrasonics are reading greater than half a tile 
      if(changeOrientationLeft == 2){ 
        changeOrientationLeft = 2.25;
      }
      if(ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2.00)+changeOrientationLeft*TILE_LENGTH) || ultrasonicAverageFront < 0){

           autoCorrect(ultrasonicAverageLeft, changeOrientationLeft, 100);
            if ((changeOrientationRight == 1) && pit == false) {
              Serial.println("changeOrientationRight == 1");
              prevOrientation = orientation;
              motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
              delayWithCorrection(ultrasonicAverageLeft, changeOrientationDown, 150, 5000);
              pit = true; 
            }
            if((changeOrientationLeft >= 1) && (orientation != prevOrientation)){
                prevOrientation = orientation;
                motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
                delayWithCorrection(ultrasonicAverageLeft, changeOrientationLeft, 150, 700);
            //              delay(250);

            }

             motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
         
         

      } else { 

        float currentTime = millis(); 
        motorControl.estop();
        orientation = UP; 
        //TODO: Test speed + turning threshold 
        motorControl.turn_right(MAX_SPEED);
        while(((millis() -currentTime) < (840+changeOrientationLeft*5))){ 
          //Serial.println(millis()-currentTime); 
//          ATAT.readTOFs(tofReadings, false); 
//          ultrasonicAverageFront = tofReadings[0]/10.0; 
//          ultrasonicAverageLeft = tofReadings[1]/10.0;
        }
        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        //delay(50); 
        motorControl.estop();

        //y = 5
        int y = currentPosition[0];
        //[4,5];
        //i = 4; i > 0; i--
        for(int i = currentPosition[1]; i > changeOrientationLeft; i--){ 
          if(abs(track[y][i]) == 1){ 
            currentPosition[0] = y; 
            currentPosition[1] = i+1; 
            break;
          }
          else { 
            track[y][i] = 1;
          }
        }
         /* int track[6][6]   {1, 1, 1, 1, 1, 1}, 
                     {1, 0, 0, 0, 0, 1},
                     {1, 0, 0, 0, 0, 1},
                     {1, 0, 0, 0, 0, 1},
                     {1, x, 1, 1, 1, 1},
                     {1, 1, 1, 1, 1, -1}
                   };
 */
        currentPosition[0] = y;
        currentPosition[1] = changeOrientationLeft; 
        //5,0
        orientation = UP; 
        changeOrientationLeft = changeOrientationLeft+1;
//        Serial.println(currentPosition[0]); 
//        Serial.println(currentPosition[1]); 
      }
    }
    else if(orientation == UP){ 
      ATAT.readTOFs(tofReadings, false);
      ultrasonicAverageFront = tofReadings[0]/10.0; 
      ultrasonicAverageLeft = tofReadings[1]/10.0; 
      if(ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2.00)+changeOrientationUp*TILE_LENGTH) || ultrasonicAverageFront < 0){

        autoCorrect(ultrasonicAverageLeft, changeOrientationUp, 100);

        if((changeOrientationUp >= 1) && (orientation != prevOrientation)){
            prevOrientation = orientation;
//            delay(250);
            motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
            delayWithCorrection(ultrasonicAverageLeft, changeOrientationUp, 150, 700);
        }

        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
      } else { 
        motorControl.estop();
        //TODO: Test speed + turning threshold 
        motorControl.turn_right(MAX_SPEED);
        float currentTime = millis(); 
        while(((millis() -currentTime) < (840+changeOrientationUp*5))){ 
//          ATAT.readTOFs(tofReadings, false); 
//          ultrasonicAverageFront = tofReadings[0]/10.0; 
//          ultrasonicAverageLeft = tofReadings[1]/10.0;
        }
        motorControl.cruise(MAX_SPEED,MAX_SPEED, 1);
        //delay(50); 
        motorControl.estop();
        //x=1
        //
        int x = currentPosition[1];
        for(int i = currentPosition[0]; i > changeOrientationUp; i--){ 
          if(abs(track[i][x]) == 1){ 
            currentPosition[0] = i+1; 
            currentPosition[1] = x; 
            break;
          }
          else { 
            track[i][x] = 1; 
//            Serial.print("[ "); 
//            Serial.print(track[i][x]);
//            Serial.print(" ]");
          }
        }
        orientation = RIGHT; 
        currentPosition[0] = changeOrientationUp;
        currentPosition[1] = x;
        changeOrientationUp++;
         /* int track[6][6]   {1, 1, 1, 1, 1, 1}, 
                     {1, x, 0, 0, 0, 1},
                     {1, 1, 0, 0, 0, 1},
                     {1, 1, 0, 0, 0, 1},
                     {1, 1, 1, 1, 1, 1},
                     {1, 1, 1, 1, 1, -1}
                   };
 */
      }
    }
    else if(orientation == RIGHT){
      ATAT.readTOFs(tofReadings, false);
      ultrasonicAverageFront = tofReadings[0]/10.0; 
      ultrasonicAverageLeft = tofReadings[1]/10.0; 
      if(ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2.00)+changeOrientationRight*TILE_LENGTH) || ultrasonicAverageFront < 0){

        autoCorrect(ultrasonicAverageLeft, changeOrientationRight, 100);

        if((changeOrientationRight >= 1) && (orientation != prevOrientation)){
            prevOrientation = orientation;
            motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
            delayWithCorrection(ultrasonicAverageLeft, changeOrientationRight, 150, 700);
//        delay(250);
        }

        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
      } else { 
        motorControl.estop();
        //changeOrientationRight++;
        //TODO: Test speed + turning threshold 
        motorControl.turn_right(MAX_SPEED);
        float currentTime = millis(); 
        while(((millis() -currentTime) < (840+changeOrientationRight*5))){ 
//          ATAT.readTOFs(tofReadings, false); 
//          ultrasonicAverageFront = tofReadings[0]/10.0; 
//          ultrasonicAverageLeft = tofReadings[1]/10.0; 
//          Serial.println("It is turning"); 
//          Serial.print("front tof reading: ");
//          Serial.println(ultrasonicAverageFront); 
//          Serial.print("left tof reading: ");
//          Serial.println(ultrasonicAverageLeft);
//          Serial.print("orientation: "); 
//          Serial.println(orientation);
        }
        motorControl.cruise(MAX_SPEED,MAX_SPEED, 1);
        delay(50); 
        motorControl.estop();
        //y=1
        int y = currentPosition[0];      //4
        for(int i = currentPosition[1]; i < (5-changeOrientationRight); i++){ 
          if(track[y][i] == 1){ 
            currentPosition[0] = y; 
            currentPosition[1] = i-1; 
            break;
          }
          else { 
            track[y][i] = 1; 
//            Serial.print("[ "); 
//            Serial.print(track[y][i]);
//            Serial.print(" ]");
          }
        }
                 /* int track[6][6]   {1, 1, 1, 1, 1, 1}, 
                     {1, 1, 1, 1, x, 1},
                     {1, 1, 0, 0, 0, 1},
                     {1, 1, 0, 0, 0, 1},
                     {1, 1, 1, 1, 1, 1},
                     {1, 1, 1, 1, 1, -1}
                   };
 */
        orientation = DOWN; 
        currentPosition[0] = y;
        currentPosition[1] = (5-changeOrientationRight);
        changeOrientationRight++;
      }
    }
    /**
     * 1,3
3,3
2,2
2,4
     */
    else if(orientation == DOWN){ 
      ATAT.readTOFs(tofReadings, false);
      ultrasonicAverageFront = tofReadings[0]/10.0; 
      ultrasonicAverageLeft = tofReadings[1]/10.0; 
//      if(track[2][3] == 0 && track[1][3] == 1 && track[3][3] == 1 && track[2][ 2] == 1 && track[2][4] == 1){ 
//        motorControl.estop(); 
//        exit(0); 
//      }
      if(ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2.00+TILE_LENGTH)+changeOrientationDown*TILE_LENGTH) || ultrasonicAverageFront < 0){
        autoCorrect(ultrasonicAverageLeft, changeOrientationDown, 100);

        
        if ((changeOrientationRight == 1) && pit == false) {
          prevOrientation = orientation;
          motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
          delayWithCorrection(ultrasonicAverageLeft, changeOrientationDown, 150, 5000);
          pit = true; 
         }
        if(changeOrientationRight == 2 && pit2 == false){ 
          prevOrientation = orientation; 
          motorControl.cruise(MAX_SPEED, MAX_SPEED, 1); 
          delayWithCorrection(ultrasonicAverageLeft, changeOrientationDown, 150, 4000);  
          pit2 = true; 
        }
        else if((changeOrientationDown > 1) && (orientation != prevOrientation)){
            prevOrientation = orientation;
            motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
            delayWithCorrection(ultrasonicAverageLeft, changeOrientationDown, 150, 700);
//            delay(250);
        }

        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
      } else { 
        motorControl.estop();
        //TODO: Test speed + turning threshold 
        motorControl.turn_right(MAX_SPEED);
        float currentTime = millis(); 
        while(((millis() -currentTime) < (840+changeOrientationDown*5))){  
//          ATAT.readTOFs(tofReadings, false); 
//          ultrasonicAverageFront = tofReadings[0]/10.0; 
//          ultrasonicAverageLeft = tofReadings[1]/10.0; 
//          Serial.println("It is turning");  
//          Serial.print("front tof reading: ");
//          Serial.println(ultrasonicAverageFront); 
//          Serial.print("left tof reading: ");
//          Serial.println(ultrasonicAverageLeft);
//          Serial.print("orientation: "); 
//          Serial.println(orientation); 
        }
        motorControl.cruise(MAX_SPEED,MAX_SPEED, 1);
        //delay(50); 
        motorControl.estop();
        //x=4
        int x = currentPosition[1]; 
        //1 ; 3; 
        for(int i = currentPosition[0]; i < (5-changeOrientationDown-1); i++){ 
          if(track[i][x] == 1){ 
            currentPosition[0] = i-1; 
            currentPosition[1] = x; 
            break;
          }
          else { 
            track[i][x] = 1; 
//            Serial.print("[ "); 
//            Serial.print(track[i][x]);
//            Serial.print(" ]"); 
          }
        }
        orientation = LEFT; 
                 /* int track[6][6]   {1, 1, 1, 1, 1, 1}, 
                     {1, 1, 1, 1, 1, 1},
                     {1, 1, 0, 0, 1, 1},
                     {1, 1, 0, 0, x, 1},
                     {1, 1, 1, 1, 1, 1},
                     {1, 1, 1, 1, 1, -1}
                   };
 */

          currentPosition[0] = (5-(changeOrientationDown-1));
          currentPosition[1] = x;
          changeOrientationDown++;
      }
    }
  }
  //ATAT.readUltrasonic(&hc, tofReadings); 
//  ATAT.readUltrasonic(&hc, tofReadings); 
//  for(int i = 0; i < NUM_TOF; i++){ 
//   Serial.println(tofReadings[i]); 
//  }
//  Serial.println("Moving foward"); 
//  //moveForward(); 
//  //delay(4000); 
//  Serial.println("Done function"); 

  //ATAT.readICM(&icm, imuReadings);
//  for(int i = 0; i < NUM_SENS; i++){ 
//    Serial.println(imuReadings[i]);
//  }
  tofReadings = NULL;
  icmReadings = NULL;
  delete[] icmReadings;
  //imuReadings = NULL;
  //delete[] imuReadings;
  delete[] tofReadings;
  
  //delay(700);
  //motorControl.estop(); 
//  while(1){ 
//    ATAT.readICM(&icm, imuReadings);
//    for(int i = 0; i < NUM_SENS; i++){ 
//      Serial.println(imuReadings[i]);
//    }
////    ATAT.readUltrasonic(&hc, tofReadings); 
////    Serial.println("In while loop"); 
////    for(int i = 0; i < NUM_TOF; i++){ 
////     Serial.println(tofReadings[i]); 
////    }
//    delay(3000); 
//  }
  //Serial.println(status);
  
}
