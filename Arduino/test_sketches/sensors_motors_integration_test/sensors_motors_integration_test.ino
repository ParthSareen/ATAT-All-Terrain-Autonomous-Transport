#include <drive.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_VL53L0X.h"
#include "sensors.h"

//Initializing Pin
// Wemos Pins
const int ML_PWM_PIN = D3;
const int ML_DIR_PIN = D4;
const int MR_PWM_PIN = D5;
const int MR_DIR_PIN = D6;

// Declare drive object
Drive drive(ML_PWM_PIN, MR_PWM_PIN, ML_DIR_PIN, MR_DIR_PIN);

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_ICM20948 icm;
Sensors sensors(2);

const int XSHUT1 = D0;
const int XSHUT2 = D7;

// For getting the private members of the drive class
float curr_speed = 0.0;
unsigned int curr_rpm = 0;
int state = 0;

void setup() {
  Serial.begin(115200);
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  Wire.begin();
  sensors.setupTOFs(&lox1, &lox2, XSHUT1, XSHUT2);
  sensors.calibrateICM(&icm);
}

void loop(void) {

  delay(50);

  Serial.println("==> Running Main()");

  // Cruise at a constant speed
  drive.cruise(HALF_SPEED, true);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  delay(2000);
  //drive.turn_right(MAX_SPEED);
//  Serial.println(curr_speed);
//  Serial.println(curr_rpm);
//  Serial.println(state);
//
//  float* tofs = new float[2];
  float* icm_reads = new float[6];
//  sensors.readTOFs(tofs, false);
//  Serial.print("TOF 1: ");
//  Serial.println(tofs[0]);
//  Serial.print("TOF 2: ");
//  Serial.println(tofs[1]);

  sensors.readICM(&icm, icm_reads);
  Serial.print("az: ");
  Serial.println(icm_reads[2]);
  Serial.print("gx: ");
  Serial.println(icm_reads[3]);
  Serial.print("gy: ");
  Serial.println(icm_reads[4]);
  Serial.print("gz: ");
  Serial.println(icm_reads[5]);
  Serial.println("");
  delay(300);

  //delete[] tofs;
  delete[] icm_reads;
}
