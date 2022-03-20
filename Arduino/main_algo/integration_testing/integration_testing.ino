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

int* echo_array = new int[D1, D2];
HCSR04 hc(D0, echo_array, NUM_US); 
Sensors ATAT(2); 


Drive motor_control(PWM_PIN_LEFT, PWM_PIN_RIGHT, DIR_PIN_LEFT, DIR_PIN_RIGHT);
float ultrasonicArrayVals[10] = {0}; 
float us_array[NUM_US] = {0, 0};
float imu_array[NUM_SENS] = {0, 0, 0, 0, 0, 0};
char ssid[] = "TP-LINK_2.4GHz_677647";    //  your network SSID (name) 
char pass[] = "98238316";   // your network password
uint16_t port  = 10101;
char* addr = "192.168.0.101";
WiFiClient  client;

Telemetry telemetry(1000, &client, addr, port);

void setup() {
  Wire.begin(); 
  Serial.begin(115200); 
  ATAT.ultrasonicSetup(D0, D1, D2);
  // put your setup code here, to run once:

//  Serial.begin(115200);
//  Serial.println();
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
//
//  Serial.println("Running Telemetry Tests");
}

void loop() {
  float curr_speed = 0.0;
  int curr_rpm = 0;
  int modeMotor = 0;
  // put your main code here, to run repeatedly:
  float * ultrasonicReadings = new float[NUM_US]; 
  float * imuReadings = new float[NUM_SENS];
//  motor_control.accelerate(ACC_MINIMAL, true);
//  motor_control.get_params(&curr_speed, &curr_rpm, &modeMotor);
//  Serial.println(curr_speed);
//  Serial.println(curr_rpm);
//  Serial.println(modeMotor);
//  delay(5000);
//  motor_control.estop(); 
//  delay(2000); 
//  motor_control.accelerate(ACC_MINIMAL, false);
//  motor_control.get_params(&curr_speed, &curr_rpm, &modeMotor);
//  Serial.println(curr_speed);
//  Serial.println(curr_rpm);
//  Serial.println(modeMotor);
//  motor_control.estop(); 
//  motor_control.turn_right(MAX_SPEED); 
//  delay(1000); 
//  motor_control.estop(); 
//  motor_control.turn_left(MAX_SPEED); 
//  delay(1000); 
//  motor_control.estop(); 
//  delay(1000);
//  ATAT.readUltrasonic(&hc, ultrasonicReadings); 

//  Serial.println("In while loop"); 

  // Parth's US shit
//  for (int i=0; i<30; i++){
//    ATAT.readUltrasonicBetter(ultrasonicReadings);
//    Serial.print("Front: ");
//    Serial.println(ultrasonicReadings[0]);
//  
//    Serial.print("Left: ");
//    Serial.println(ultrasonicReadings[1]);
//    delay(100);
//  }
//  delay(1000);
  


  ATAT.readICM(&icm, imuReadings);
  for(int i = 0; i < NUM_SENS; i++){ 
    Serial.println(imuReadings[i]);
  }
  delay(1000);
  ultrasonicReadings = NULL;
  imuReadings = NULL;
  delete[] imuReadings;
  delete[] ultrasonicReadings; 

  

}
