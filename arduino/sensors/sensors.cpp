#include "sensors.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"
#include <HCSR04.h>

#define OUTPUT_READABLE_ACCELGYRO

// ============== Public Methods ==============

Sensors::Sensors(int trig_pin, int echo_pin[], int num_sensors);{
  HCSR04 hc(trig_pin, echo_pin, num_sensors);
  MPU6050 accelgyro(0x68);
}

/*
* Calibrating Ultrasonic Sensor
* Function: Reads 10 sensor readings and averages - then check to see if it fits in tolerance
* Returns: Bool of within tolerance or not
*/
bool Sensors::calibrateUltrasonic(int num_readings){
	Serial.println("==> Calibrating Ultrasonic");
  float averages[num_sensors];
  bool status

  // Read each sensor ten times and find the average
  for (int i = 0; i < num_readings; i++)
    averages[i] = hc.dist(i);

  for (int i = 0; i < num_sensors; i++)
    averages[i] = averages[i]/num_readings;
    if ((averages[i] < 4.5) || (averages[i] > 5.5))
      return false;
  return true;
}

/**
* Calibrating IMU
* Function: Calibrates IMU
* Returns: Integer Array of Offsets for Accelero/Gyro
*/
int16_t * Sensors::calibrateIMU(){
	Serial.println("==> Calibrating IMU");
  ///////////////////////////////////   CONFIGURATION   /////////////////////////////
  //Change this 3 variables if you want to fine tune the skecth to your needs.
  int buffersize=1000;     //Amount of readings used to average, make it higher to get more precision but sketch will be slower  (default:1000)
  int acel_deadzone=8;     //Acelerometer error allowed, make it lower to get more precision, but sketch may not converge  (default:8)
  int giro_deadzone=1;     //Giro error allowed, make it lower to get more precision, but sketch may not converge  (default:1)

  int16_t ax, ay, az,gx, gy, gz;

  int mean_ax,mean_ay,mean_az,mean_gx,mean_gy,mean_gz,state=0;
  int ax_offset,ay_offset,az_offset,gx_offset,gy_offset,gz_offset;
  int calibrations[6];

  accelgyro.setXAccelOffset(0);
  accelgyro.setYAccelOffset(0);
  accelgyro.setZAccelOffset(0);
  accelgyro.setXGyroOffset(0);
  accelgyro.setYGyroOffset(0);
  accelgyro.setZGyroOffset(0);

  if (state==0){
    Serial.println("\nReading sensors for first time...");
    meansensors();
    state++;
    delay(1000);
  }

  if (state==1) {
    Serial.println("\nCalculating offsets...");
    calibration();
    state++;
    delay(1000);
  }

  if (state==2) {
    meansensors();
    Serial.println("\nFINISHED!");
    Serial.print("\nSensor readings with offsets:\t");
    Serial.print(mean_ax); 
    Serial.print("\t");
    Serial.print(mean_ay); 
    Serial.print("\t");
    Serial.print(mean_az); 
    Serial.print("\t");
    Serial.print(mean_gx); 
    Serial.print("\t");
    Serial.print(mean_gy); 
    Serial.print("\t");
    Serial.println(mean_gz);
    Serial.print("Your offsets:\t");
    Serial.print(ax_offset); 
    Serial.print("\t");
    Serial.print(ay_offset); 
    Serial.print("\t");
    Serial.print(az_offset); 
    Serial.print("\t");
    Serial.print(gx_offset); 
    Serial.print("\t");
    Serial.print(gy_offset); 
    Serial.print("\t");
    Serial.println(gz_offset); 
    calibrations = {ax_offset, ay_offset, az_offset, gx_offset, gy_offset, gz_offset};
    return calibrations;
  }
}

/**
* Reading Ultrasonic Sensor
* Function: Reads Ultrasonic Values
* Returns: Integer Array of Ultrasonic Values
*/
float * Sensors::readUltrasonic(){
  Serial.println("==> Reading Ultrasonic Sensor");
  static float ultrasonic_readings[num_sensors]
  for (int i = 0; i < num_sensors; i++)
    ultrasonic_readings[i] = hc.dist(i);
  return ultrasonic_readings
}

/**
* Reading IMU Values
* Function: Read IMU Values
* Returns: Array of IMU Values
*/
int16_t * Sensors::readIMU(int speed = MAX_SPEED){
    Serial.println("==> Reading IMU");

    int16_t ax, ay, az;
    int16_t gx, gy, gz;
    

    Serial.println("Updating internal sensor offsets...");
    accelgyro.setXAccelOffset(1416);
    accelgyro.setYAccelOffset(1414);
    accelgyro.setZAccelOffset(1446);
    accelgyro.setXGyroOffset(92);
    accelgyro.setYGyroOffset(22);
    accelgyro.setZGyroOffset(76);
    Serial.println("Updating internal sensor offsets complete...");

    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int16_t IMU_Values[6] = {ax, ay, az, gx, gy, gz}

    #ifdef OUTPUT_READABLE_ACCELGYRO
      // display tab-separated accel/gyro x/y/z values
      Serial.print("a/g:\t");
      Serial.print(ax); Serial.print("\t");
      Serial.print(ay); Serial.print("\t");
      Serial.print(az); Serial.print("\t");
      Serial.print(gx); Serial.print("\t");
      Serial.print(gy); Serial.print("\t");
      Serial.println(gz);
      delay(1000);
    #endif
    delay(1000);

    return IMU_Values;
}

// ============== Helper Methods ==============
void meansensors(){
  long i=0,buff_ax=0,buff_ay=0,buff_az=0,buff_gx=0,buff_gy=0,buff_gz=0;

  while (i<(buffersize+101)){
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    if (i>100 && i<=(buffersize+100)){ //First 100 measures are discarded
      buff_ax=buff_ax+ax;
      buff_ay=buff_ay+ay;
      buff_az=buff_az+az;
      buff_gx=buff_gx+gx;
      buff_gy=buff_gy+gy;
      buff_gz=buff_gz+gz;
    }
    if (i==(buffersize+100)){
      mean_ax=buff_ax/buffersize;
      mean_ay=buff_ay/buffersize;
      mean_az=buff_az/buffersize;
      mean_gx=buff_gx/buffersize;
      mean_gy=buff_gy/buffersize;
      mean_gz=buff_gz/buffersize;
    }
    i++;
    delay(2); //Needed so we don't get repeated measures
  }
}

void calibration(){
  ax_offset=-mean_ax/8;
  ay_offset=-mean_ay/8;
  az_offset=(16384-mean_az)/8;

  gx_offset=-mean_gx/4;
  gy_offset=-mean_gy/4;
  gz_offset=-mean_gz/4;
  while (1){
    int ready=0;
    accelgyro.setXAccelOffset(ax_offset);
    accelgyro.setYAccelOffset(ay_offset);
    accelgyro.setZAccelOffset(az_offset);

    accelgyro.setXGyroOffset(gx_offset);
    accelgyro.setYGyroOffset(gy_offset);
    accelgyro.setZGyroOffset(gz_offset);

    meansensors();
    Serial.println("...");

    if (abs(mean_ax)<=acel_deadzone) ready++;
    else ax_offset=ax_offset-mean_ax/acel_deadzone;

    if (abs(mean_ay)<=acel_deadzone) ready++;
    else ay_offset=ay_offset-mean_ay/acel_deadzone;

    if (abs(16384-mean_az)<=acel_deadzone) ready++;
    else az_offset=az_offset+(16384-mean_az)/acel_deadzone;

    if (abs(mean_gx)<=giro_deadzone) ready++;
    else gx_offset=gx_offset-mean_gx/(giro_deadzone+1);

    if (abs(mean_gy)<=giro_deadzone) ready++;
    else gy_offset=gy_offset-mean_gy/(giro_deadzone+1);

    if (abs(mean_gz)<=giro_deadzone) ready++;
    else gz_offset=gz_offset-mean_gz/(giro_deadzone+1);

    if (ready==6) break;
  }
}