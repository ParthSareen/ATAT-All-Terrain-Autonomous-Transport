#include <drive.h>

//Initializing Pin
const int ML_PWM_PIN = 11;
const int ML_DIR_PIN = 8;
const int MR_PWM_PIN = 10;
const int MR_DIR_PIN = 7;

int set = 0;

// Declare drive object
Drive drive(ML_PWM_PIN, MR_PWM_PIN, ML_DIR_PIN, MR_DIR_PIN);

void setup() {
  Serial.begin(9600);
}

void loop() {

  // For getting the private members of the drive class
  float curr_speed = 1.0;
  int curr_rpm = 1;
  int mode = 1;

 // Wait before running
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

  // Testing accelerating 
  drive.accelerate(ACC_MINIMAL, true);
  drive.get_params(&curr_speed, &curr_rpm, &mode);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(mode);

  delay(5000);
  
  // Test decceleration
  drive.deccelerate(ACC_MINIMAL, true);
  drive.get_params(&curr_speed, &curr_rpm, &mode);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(mode);

  delay(5000);

  // Turn right
  drive.turn_right(HALF_SPEED);
  drive.get_params(&curr_speed, &curr_rpm, &mode);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(mode);

  delay(5000);

  // Turn left
  drive.turn_left(HALF_SPEED);
  drive.get_params(&curr_speed, &curr_rpm, &mode);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(mode);

  delay(5000);

  // Cruise at a constant speed
  drive.cruise(HALF_SPEED, true);
  drive.get_params(&curr_speed, &curr_rpm, &mode);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(mode);

  delay(5000);

  // E-stop
  drive.estop();
  drive.get_params(&curr_speed, &curr_rpm, &mode);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(mode);

  delay(5000); 
}
