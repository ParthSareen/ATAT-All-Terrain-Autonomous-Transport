#include <drive.h>

//Initializing Pin
const int ML_PWM_PIN = 11;
const int ML_DIR_PIN = 8;
const int MR_PWM_PIN = 10;
const int MR_DIR_PIN = 7;
const int R_ENCODER_PIN = 13;
const int L_ENCODER_PIN = 2;

int set = 0;

unsigned long lastTime;
float *encoderValues;

// Declare drive object
Drive drive(ML_PWM_PIN, MR_PWM_PIN, ML_DIR_PIN, MR_DIR_PIN, L_ENCODER_PIN, R_ENCODER_PIN);

void setup() {
  Serial.begin(9600);
  lastTime = millis();
}

void loop() {

  // For getting the private members of the drive class
  float curr_speed = 0.0;
  int curr_rpm = 0;
  int mode = 0;

 // Wait before running
 // Send 1 through serial monitor to start
  while(set == 0){
    Serial.println("==> Waiting ");
    delay(50);
    set = Serial.parseInt();
    Serial.print(set);
    if(set != 0){
      break;
    }
  }

  Serial.println("==> Running Main()");
//
//  // Testing accelerating 
//  drive.accelerate(ACC_MINIMAL, true);
//  drive.get_params(&curr_speed, &curr_rpm, &mode);
//  Serial.println(curr_speed);
//  Serial.println(curr_rpm);
//  Serial.println(mode);
//
//  delay(5000);
//  
//  // Test decceleration
//  drive.deccelerate(ACC_MINIMAL, true);
//  drive.get_params(&curr_speed, &curr_rpm, &mode);
//  Serial.println(curr_speed);
//  Serial.println(curr_rpm);
//  Serial.println(mode);
//
//  delay(5000);
//
//  // Turn right
//  drive.turn_right(HALF_SPEED);
//  drive.get_params(&curr_speed, &curr_rpm, &mode);
//  Serial.println(curr_speed);
//  Serial.println(curr_rpm);
//  Serial.println(mode);
//
//  delay(5000);
//
//  // Turn left
//  drive.turn_left(HALF_SPEED);
//  drive.get_params(&curr_speed, &curr_rpm, &mode);
//  Serial.println(curr_speed);
//  Serial.println(curr_rpm);
//  Serial.println(mode);
//
//  delay(5000);

  // Cruise at a constant speed
  drive.cruise(MAX_SPEED, true);
  drive.get_params(&curr_speed, &curr_rpm, &mode);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(mode);
  
  delay(5000);
//
//  // E-stop
//  drive.estop();
//  drive.get_params(&curr_speed, &curr_rpm, &mode);
//  Serial.println(curr_speed);
//  Serial.println(curr_rpm);
//  Serial.println(mode);
//
//  delay(5000); 
  Serial.println(mode);
  encoderValues = drive.encoderReading(&lastTime, HALF_SPEED);
  for ( int i = 0; i < 2; i++ ) {
    Serial.println("Encoder value");
    Serial.println(*(encoderValues + i));
   }
  drive.get_params(&curr_speed, &curr_rpm, &mode);
  delay(5000); 
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(mode);
}