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

//#include <telemetry.pb.h>
//#include <ESP8266WiFi.h>
//#include <pb.h>
//#include <pb_common.h>
//#include <pb_decode.h>
//#include <pb_encode.h>
//#include <Telemetry.h>
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
#define FRONT_US_DIST 7.00
#define TILE_LENGTH 30.5
Adafruit_ICM20948 icm;

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

char ssid[] = "TP-LINK_2.4GHz_677647";
char pass[] = "98238316";  
char* addr = "192.168.0.108";
uint16_t port  = 10101;

//WiFiClient  client;

//Telemetry telemetry(1000, &client, addr, port);
int count = 0;
Sensors ATAT(2);
long duration;
int distance;
//Setting up Ultrasonic and IMU and motor
Drive motorControl(PWM_PIN_LEFT, PWM_PIN_RIGHT, DIR_PIN_LEFT, DIR_PIN_RIGHT); 

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
                     {0, 0, 0, 0, 0, -1}
                   };
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

bool testState = 0; 

float ultrasonicAverageFront = 0; 
float ultrasonicAverageLeft = 0; 

float lastUltrasonicAverageFront = 0; 
float lastUltrasonicAverageLeft = 0; 

void setup() {
  //Wire.begin(D1, D2);
  Serial.begin(115200);
  //Serial.println();
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  Wire.begin();
//  Serial.print("Setting up WIFI for SSID ");
//  Serial.println(ssid);
//
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, pass);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    Serial.println("WIFI connection failed, reconnecting...");
//    delay(500);
//  }
//
//  Serial.println("");
//  Serial.print("WiFi connected, ");
//  Serial.print("IP address: ");
//  Serial.println(WiFi.localIP());

  //TODO: readd setup tofs when work !! 
  ATAT.setupTOFs(&lox1, &lox2, XSHUT1, XSHUT2); 
  Serial.println("Calibrating ICM");
  ATAT.calibrateICM(&icm);
  //delay(5000);
}

void loop() {
  float * tofReadings = new float[NUM_TOF]; 
  float * icmReadings = new float [NUM_SENS]; 
  ATAT.readTOFs(tofReadings, false);  
  
  //float * imuReadings = new float[NUM_SENS];
  //take 10 readings find average 
//  for(int i = 0; i < 10; i++){ 
//    //TODO: Read ultrasonic method will change 
//    ATAT.readUltrasonic(&hc, tofReadings); 
//    ultrasonicAverageFront += tofReadings[0]/10.00; 
//    ultrasonicAverageLeft += tofReadings[1]/10.00; 
//  }

  //This is 40ms read 
  //ATAT.readUltrasonicBetter(tofReadings); 
  ultrasonicAverageFront = tofReadings[0]/10.0; 
  ultrasonicAverageLeft = tofReadings[1]/10.0; 
  Serial.print("front tof reading: ");
  Serial.println(ultrasonicAverageFront); 
  Serial.print("left tof reading: ");
  Serial.println(ultrasonicAverageLeft);
  ATAT.readICM(&icm, icmReadings); 
//  if (count == 5){
//    telemetry.uploadUltrasonic(ultrasonicAverageFront, ultrasonicAverageLeft, 0);
//    count = 0;
//  }
//   else {
//    count++;   
//  }
  //check if it is in a pit, if it is ignore readings. the variable Gy corresponds to pit readings.  
  if(icmReadings[4] < 0.11 && icmReadings[4] > -0.11){
    if(orientation == LEFT){  
      //index check 
        //Check if current pos is visited, check if next pos in orientation is unvisited, check if ultrasonics are reading greater than half a tile 
      if(track[currentPosition[0]][currentPosition[1]-1] == 0 && (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2.00)+changeOrientationLeft*TILE_LENGTH)) || ultrasonicAverageFront < 0){
  //         Serial.println(ultrasonicAverageFront); 
  //         Serial.println(ultrasonicAverageLeft); 
           //Serial.println("inside if");
           motorControl.cruise(MAX_SPEED, 1); 
           Serial.print("orientation: "); 
           Serial.println(orientation); 
           //track[currentPosition[0]][currentPosition[1]-1] = 1; 
      } else { 
  //      Serial.println(ultrasonicAverageFront); 
  //      Serial.println(ultrasonicAverageLeft);
        Serial.println("move into else"); 
        float currentTime = millis(); 
        motorControl.estop();
        orientation = UP; 
        //TODO: Test speed + turning threshold 
        motorControl.turn_right(HALF_SPEED);
        while(ultrasonicAverageFront < 70 && ((millis() -currentTime) < 670 )){ 
          Serial.println(millis()-currentTime); 
          ATAT.readTOFs(tofReadings, false); 
          ultrasonicAverageFront = tofReadings[0]/10.0; 
          ultrasonicAverageLeft = tofReadings[1]/10.0; 
          Serial.println("It is turning"); 
          Serial.print("front tof reading: ");
          Serial.println(ultrasonicAverageFront); 
          Serial.print("left tof reading: ");
          Serial.println(ultrasonicAverageLeft);
          Serial.print("orientation: "); 
          Serial.println(orientation);
//          ATAT.readICM(&icm, icmReadings);
//          Serial.print("ax: "); 
//          Serial.println(icmReadings[0]); 
//          Serial.print("ay: "); 
//          Serial.println(icmReadings[1]);
//          Serial.print("az: ");
//          Serial.println(icmReadings[2]);
//          Serial.print("gx: ");
//          Serial.println(icmReadings[3]);
//          Serial.print("gy: ");
//          Serial.println(icmReadings[4]);
//          Serial.print("gz: ");
//          Serial.println(icmReadings[5]);
          
        }
        motorControl.estop();

        //4
        int y = currentPosition[0];
        //[4,5];
        for(int i = currentPosition[1]; i > changeOrientationLeft; i--){ 
          if(abs(track[y][i]) == 1){ 
            currentPosition[0] = y; 
            currentPosition[1] = i+1; 
            break;
          }
          else { 
            track[y][i] = 1; 
            Serial.print("[ "); 
            Serial.print(track[y][i]);
            Serial.print(" ]"); 
            //[5][0] = 1
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
        changeOrientationLeft++;
        Serial.println(currentPosition[0]); 
        Serial.println(currentPosition[1]); 
      }
    }
    else if(orientation == UP){ 
      if(track[currentPosition[0]-1][currentPosition[1]] == 0 &&  (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2.00)+changeOrientationUp*TILE_LENGTH)) || ultrasonicAverageFront < 0){
          motorControl.cruise(MAX_SPEED, 1);
      } else { 
        motorControl.estop();
        orientation = RIGHT; 
        //TODO: Test speed + turning threshold 
        motorControl.turn_right(HALF_SPEED);
        float currentTime = millis(); 
        while(ultrasonicAverageFront < 70 && ((millis() -currentTime) < 670 )){ 
          ATAT.readTOFs(tofReadings, false); 
          ultrasonicAverageFront = tofReadings[0]/10.0; 
          ultrasonicAverageLeft = tofReadings[1]/10.0; 
          Serial.println("It is turning"); 
          Serial.print("front tof reading: ");
          Serial.println(ultrasonicAverageFront); 
          Serial.print("left tof reading: ");
          Serial.println(ultrasonicAverageLeft);
          Serial.print("orientation: "); 
          Serial.println(orientation);
        }
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
            Serial.print("[ "); 
            Serial.print(track[i][x]);
            Serial.print(" ]");
          }
        }
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
      if(track[currentPosition[0]][currentPosition[1]] == 1 && track[currentPosition[0]][currentPosition[1]+1] == 0 &&  (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2.00)+changeOrientationRight*TILE_LENGTH))|| ultrasonicAverageFront < 0){
          motorControl.cruise(MAX_SPEED, 1);
      } else { 
        motorControl.estop();
        orientation = DOWN; 
        //changeOrientationRight++;
        //TODO: Test speed + turning threshold 
        motorControl.turn_right(HALF_SPEED);
        float currentTime = millis(); 
        while(ultrasonicAverageFront < 70 && ((millis() -currentTime) < 670 )){ 
          ATAT.readTOFs(tofReadings, false); 
          ultrasonicAverageFront = tofReadings[0]/10.0; 
          ultrasonicAverageLeft = tofReadings[1]/10.0; 
          Serial.println("It is turning"); 
          Serial.print("front tof reading: ");
          Serial.println(ultrasonicAverageFront); 
          Serial.print("left tof reading: ");
          Serial.println(ultrasonicAverageLeft);
          Serial.print("orientation: "); 
          Serial.println(orientation);
        }
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
            Serial.print("[ "); 
            Serial.print(track[y][i]);
            Serial.print(" ]");
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
      if(track[2][3] == 0 && track[1][3] == 1 && track[3][3] == 1 && track[2][2] == 1 && track[2][4] == 1){ 
        motorControl.estop(); 
        exit(0); 
      }
      else if(track[currentPosition[0]+1][currentPosition[1]] == 0 &&  (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH/2.00)+changeOrientationDown*TILE_LENGTH))|| ultrasonicAverageFront < 0){
          motorControl.cruise(MAX_SPEED, 1);
      } else { 
        motorControl.estop();
        orientation = LEFT; 
        //TODO: Test speed + turning threshold 
        motorControl.turn_right(HALF_SPEED);
        float currentTime = millis(); 
        while(ultrasonicAverageFront < 70 && ((millis() -currentTime) < 670 )){  
          ATAT.readTOFs(tofReadings, false); 
          ultrasonicAverageFront = tofReadings[0]/10.0; 
          ultrasonicAverageLeft = tofReadings[1]/10.0; 
          Serial.println("It is turning"); 
          Serial.print("front tof reading: ");
          Serial.println(ultrasonicAverageFront); 
          Serial.print("left tof reading: ");
          Serial.println(ultrasonicAverageLeft);
          Serial.print("orientation: "); 
          Serial.println(orientation);
        }
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
            Serial.print("[ "); 
            Serial.print(track[i][x]);
            Serial.print(" ]"); 
          }
        }
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
  //imuReadings = NULL;
  //delete[] imuReadings;
  delete[] tofReadings; 
  //delay(1000);
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
