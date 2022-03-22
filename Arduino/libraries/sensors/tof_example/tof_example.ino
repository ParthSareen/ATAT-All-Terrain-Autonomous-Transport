#include "sensors.h"
#include "DFRobot_I2CMultiplexer.h"
#include "Adafruit_VL53L0X.h"

DFRobot_I2CMultiplexer I2CMulti(0x70);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Sensors sensors(2);
bool debug = true;

void setup() {
  sensors.setupTOFs(&I2CMulti, &lox);
}

void loop() {
  float * tof_readings = new float[2]; 
  sensors.readTOFs(tof_readings, debug);
  delete[] tof_readings;
}
