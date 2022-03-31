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
#define TILE_LENGTH 28

// Measured
#define CORRECTION_THRESHOLD_UPPER 17
#define CORRECTION_THRESHOLD_LOWER 12

#define TURN_ANGLE_UPPER_THRESHOLD 1.65806
#define TURN_ANGLE_LOWER_THRESHOLD 1.48353

Adafruit_ICM20948 icm;

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();

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
int prevOrientation = DOWN;

int track[6][6] = {  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, -1}
};

int currentPosition[2] = {5, 4};

bool testState = 0;

float ultrasonicAverageFront = 0;
float ultrasonicAverageLeft = 0;

// For average tof values
float tofMeanFront = 0;
float tofMeanLeft = 0;
int tofFrontValid = 0;
int tofLeftValid = 0;

float lastUltrasonicAverageFront = 0;
float lastUltrasonicAverageLeft = 0;

void setup() {
  // Serial.begin(115200);
  // Serial.println("Setup");

  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  Wire.begin();

  ATAT.setupTOFs(&lox1, &lox2, XSHUT1, XSHUT2);
  ATAT.calibrateICM(&icm);
  delay(5000);

}

void loop() {

  float * tofReadings = new float[NUM_TOF];
  float * icmReadings = new float [NUM_SENS];

  for (int i = 0; i < 5; i++) {
    ATAT.readTOFs(tofReadings, false);

    if (tofReadings[0] > 0){
      tofMeanFront += tofReadings[0];
      tofFrontValid++;
    }

    if (tofReadings[1] > 0){
      tofMeanLeft += tofReadings[1];
      tofLeftValid++;
    }

  }
 
  ultrasonicAverageFront = (tofMeanFront / tofFrontValid) / 10.0;
  ultrasonicAverageLeft = (tofMeanLeft / tofLeftValid) / 10.0;
  ATAT.readICM(&icm, icmReadings);

  if (icmReadings[4] < 0.2 && icmReadings[4] > -0.2) {
    if (orientation == LEFT) {
      ATAT.readTOFs(tofReadings, false);
      ultrasonicAverageFront = tofReadings[0] / 10.0;
      ultrasonicAverageLeft = tofReadings[1] / 10.0;
      //index check
      //Check if current pos is visited, check if next pos in orientation is unvisited, check if ultrasonics are reading greater than half a tile
      if (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH / 2.00) + changeOrientationLeft * TILE_LENGTH) || ultrasonicAverageFront < 0) {

        // Correction - TODO: Calibrate
        if ((ultrasonicAverageLeft > (CORRECTION_THRESHOLD_UPPER + TILE_LENGTH * changeOrientationLeft)) && (ultrasonicAverageLeft != -0.2)) {
          motorControl.cruise(MAX_SPEED, HIGH_SPEED, 1);
          delay(100);
        }

        else if ((ultrasonicAverageLeft < CORRECTION_THRESHOLD_LOWER + TILE_LENGTH * changeOrientationLeft) && (ultrasonicAverageLeft != -0.2)) {
          motorControl.cruise(HIGH_SPEED, MAX_SPEED, 1);
          delay(100);
        }

        else {
          motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        }

        if ((changeOrientationLeft >= 1) && (orientation != prevOrientation)) {
          prevOrientation = orientation;
          delay(250);
        }

        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);

      } else {
        
        float currentTime = millis();
        float sampleTime = millis();
        float timeDelta = 0;
        float currentAngle = 0; 

        motorControl.estop();
        orientation = UP;
        motorControl.turn_right(MAX_SPEED);

        // Previous time delay was 720
        while (((millis() - currentTime) < (1000 + changeOrientationLeft * 5)) &&  (currentAngle < TURN_ANGLE_LOWER_THRESHOLD)) {
          // ATAT.readTOFs(tofReadings, false);
          // ultrasonicAverageFront = tofReadings[0] / 10.0;
          // ultrasonicAverageLeft = tofReadings[1] / 10.0;

          ATAT.readICM(&icm, icmReadings); // Read ICM
          sampleTime = millis(); // New current time
          timeDelta = sampleTime - currentTime; 
          currentAngle += abs(timeDelta*(icmReadings[4]/1000.0)); // Determine angle in rad

        }
        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        motorControl.estop();

        //y = 5
        int y = currentPosition[0];
        //[4,5];
        //i = 4; i > 0; i--
        for (int i = currentPosition[1]; i > changeOrientationLeft; i--) {
          if (abs(track[y][i]) == 1) {
            currentPosition[0] = y;
            currentPosition[1] = i + 1;
            break;
          }
          else {
            track[y][i] = 1;
          }
        }
        currentPosition[0] = y;
        currentPosition[1] = changeOrientationLeft;
        //5,0
        orientation = UP;
        changeOrientationLeft = changeOrientationLeft + 1;
      }
    }
    else if (orientation == UP) {
      ATAT.readTOFs(tofReadings, false);
      ultrasonicAverageFront = tofReadings[0] / 10.0;
      ultrasonicAverageLeft = tofReadings[1] / 10.0;
      if (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH / 2.00) + changeOrientationUp * TILE_LENGTH) || ultrasonicAverageFront < 0) {

        if ((ultrasonicAverageLeft > (CORRECTION_THRESHOLD_UPPER + TILE_LENGTH * changeOrientationUp)) && (ultrasonicAverageLeft != -0.2)) {
          motorControl.cruise(MAX_SPEED, HIGH_SPEED, 1);
          delay(100);
        }

        else if ((ultrasonicAverageLeft < CORRECTION_THRESHOLD_LOWER + TILE_LENGTH * changeOrientationUp) && (ultrasonicAverageLeft != -0.2)) {
          motorControl.cruise(HIGH_SPEED, MAX_SPEED, 1);
          delay(100);
        }

        else {
          motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        }

        if ((changeOrientationUp >= 1) && (orientation != prevOrientation)) {
          prevOrientation = orientation;
          delay(250);
        }

        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
      } else {
        motorControl.estop();
        //TODO: Test speed + turning threshold
        motorControl.turn_right(MAX_SPEED);
        float currentTime = millis();
        while (((millis() - currentTime) < (720 + changeOrientationUp * 10) )) {
          ATAT.readTOFs(tofReadings, false);
          ultrasonicAverageFront = tofReadings[0] / 10.0;
          ultrasonicAverageLeft = tofReadings[1] / 10.0;
        }
        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        motorControl.estop();
        //x=1
        //
        int x = currentPosition[1];
        for (int i = currentPosition[0]; i > changeOrientationUp; i--) {
          if (abs(track[i][x]) == 1) {
            currentPosition[0] = i + 1;
            currentPosition[1] = x;
            break;
          }
          else {
            track[i][x] = 1;
          }
        }
        orientation = RIGHT;
        currentPosition[0] = changeOrientationUp;
        currentPosition[1] = x;
        changeOrientationUp++;
      }
    }
    else if (orientation == RIGHT) {
      ATAT.readTOFs(tofReadings, false);
      ultrasonicAverageFront = tofReadings[0] / 10.0;
      ultrasonicAverageLeft = tofReadings[1] / 10.0;
      if (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH / 2.00) + changeOrientationRight * TILE_LENGTH) || ultrasonicAverageFront < 0) {

        if ((ultrasonicAverageLeft > (CORRECTION_THRESHOLD_UPPER + TILE_LENGTH * changeOrientationRight)) && (ultrasonicAverageLeft != -0.2)) {
          motorControl.cruise(MAX_SPEED, HIGH_SPEED, 1);
          delay(100);
        }

        else if ((ultrasonicAverageLeft < CORRECTION_THRESHOLD_LOWER + TILE_LENGTH * changeOrientationRight) && (ultrasonicAverageLeft != -0.2)) {
          motorControl.cruise(HIGH_SPEED, MAX_SPEED, 1);
          delay(100);
        }

        else {
          motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        }

        if ((changeOrientationRight >= 1) && (orientation != prevOrientation)) {
          prevOrientation = orientation;
          delay(250);
        }

        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
      } else {
        motorControl.estop();

        //TODO: Test speed + turning threshold
        motorControl.turn_right(MAX_SPEED);
        float currentTime = millis();
        while ((millis() - currentTime) < (720 + changeOrientationRight * 10) ) {
          ATAT.readTOFs(tofReadings, false);
          ultrasonicAverageFront = tofReadings[0] / 10.0;
          ultrasonicAverageLeft = tofReadings[1] / 10.0;

        }
        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        delay(50);
        motorControl.estop();
        //y=1
        int y = currentPosition[0];      //4
        for (int i = currentPosition[1]; i < (5 - changeOrientationRight); i++) {
          if (track[y][i] == 1) {
            currentPosition[0] = y;
            currentPosition[1] = i - 1;
            break;
          }
          else {
            track[y][i] = 1;
          }
        }

        orientation = DOWN;
        currentPosition[0] = y;
        currentPosition[1] = (5 - changeOrientationRight);
        changeOrientationRight++;
      }
    }
    /**
     * 1,3
    3,3
    2,2
    2,4
     */
    else if (orientation == DOWN) {
      ATAT.readTOFs(tofReadings, false);
      ultrasonicAverageFront = tofReadings[0] / 10.0;
      ultrasonicAverageLeft = tofReadings[1] / 10.0;

      if (ultrasonicAverageFront > ((FRONT_US_DIST + TILE_LENGTH / 2.00 + TILE_LENGTH) + changeOrientationDown * TILE_LENGTH) || ultrasonicAverageFront < 0) {

        if ((ultrasonicAverageLeft > (CORRECTION_THRESHOLD_UPPER + TILE_LENGTH * changeOrientationDown)) && (ultrasonicAverageLeft != -0.2)) {
          motorControl.cruise(MAX_SPEED, HIGH_SPEED, 1);
          delay(100);
        }

        else if ((ultrasonicAverageLeft < CORRECTION_THRESHOLD_LOWER + TILE_LENGTH * changeOrientationDown) && (ultrasonicAverageLeft != -0.2)) {
          motorControl.cruise(HIGH_SPEED, MAX_SPEED, 1);
          delay(100);
        }

        else {
          motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        }

        if ((changeOrientationDown >= 1) && (orientation != prevOrientation)) {
          prevOrientation = orientation;
          delay(250);
        }

        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
      } else {
        motorControl.estop();
        //TODO: Test speed + turning threshold
        motorControl.turn_right(MAX_SPEED);
        float currentTime = millis();
        while (((millis() - currentTime) < (720 + changeOrientationDown * 10) )) {
          ATAT.readTOFs(tofReadings, false);
          ultrasonicAverageFront = tofReadings[0] / 10.0;
          ultrasonicAverageLeft = tofReadings[1] / 10.0;

        }
        motorControl.cruise(MAX_SPEED, MAX_SPEED, 1);
        //delay(50);
        motorControl.estop();
        //x=4
        int x = currentPosition[1];
        //1 ; 3;
        for (int i = currentPosition[0]; i < (5 - changeOrientationDown - 1); i++) {
          if (track[i][x] == 1) {
            currentPosition[0] = i - 1;
            currentPosition[1] = x;
            break;
          }
          else {
            track[i][x] = 1;
          }
        }
        orientation = LEFT;

        currentPosition[0] = (5 - (changeOrientationDown - 1));
        currentPosition[1] = x;
        changeOrientationDown++;
      }
    }
  }

  tofReadings = NULL;
  icmReadings = NULL;
  delete[] icmReadings;
  delete[] tofReadings;

}
