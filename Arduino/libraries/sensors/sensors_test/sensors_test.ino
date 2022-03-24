#include <Adafruit_ICM20X.h>
#include <Adafruit_ICM20948.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_VL53L0X.h"

#include "sensors.h"

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_ICM20948 icm;
Sensors sensors(2);

const int XSHUT1 = D0;
const int XSHUT2 = D7;
// D8 Does not work
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(XSHUT1, OUTPUT);
  pinMode(XSHUT2, OUTPUT);
  Wire.begin();
  sensors.setupTOFs(&lox1, &lox2, XSHUT1, XSHUT2);
  sensors.calibrateICM(&icm);
}

void loop() {

  float* tofs = new float[2];
  float* icm_reads = new float[6];
  sensors.readTOFs(tofs, false);
  Serial.print("TOF 1: ");
  Serial.println(tofs[0]);
  Serial.print("TOF 2: ");
  Serial.println(tofs[1]);
  

  sensors.readICM(&icm, icm_reads);
  Serial.print("az: ");
  Serial.println(icm_reads[2]);
  Serial.println("");
  delay(300);
  
  delete[] tofs;
  delete[] icm_reads;
}
