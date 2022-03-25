#include <drive.h>

//Initializing Pin
// Arduino Pins
//const int ML_PWM_PIN = 3;
//const int ML_DIR_PIN = 4;
//const int MR_PWM_PIN = 6;
//const int MR_DIR_PIN = 7;

// Wemos Pins
const int ML_PWM_PIN = D3;
const int ML_DIR_PIN = D4;
const int MR_PWM_PIN = D5;
const int MR_DIR_PIN = D6;

int set = 0;

// Declare drive object
Drive drive(ML_PWM_PIN, MR_PWM_PIN, ML_DIR_PIN, MR_DIR_PIN);

void setup() {
  Serial.begin(115200);
}

void loop(void) {

  // For getting the private members of the drive class
  float curr_speed = 0.0;
  unsigned int curr_rpm = 0;
  int state = 0;

  delay(50);

// Wait before running
//Send 1 through serial monitor to start
//  while(set == 0){
//    Serial.println("==> Waiting ");
//    delay(50);
//    set = Serial.parseInt();
//    Serial.print(set);
//    if(set != 0){
//      break;
//    }
//  }

  Serial.println("==> Running Main()");

  // Testing accelerating 
  drive.accelerate(ACC_MINIMAL, true);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

  // Turn left
  drive.turn_left(HALF_SPEED);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

  // Turn right
  drive.turn_right(HALF_SPEED);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

    // Test decceleration
  drive.deccelerate(ACC_MINIMAL, true);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

    // Turn left
  drive.turn_left(HALF_SPEED);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

    // Turn right
  drive.turn_right(HALF_SPEED);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

    // Cruise at a constant speed
  drive.cruise(HALF_SPEED, true);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

  // Cruise at a constant speed
  drive.cruise(HALF_SPEED, true);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

  // E-stop
  drive.estop();
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

  // Turn left
  drive.turn_left(HALF_SPEED);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

  // Turn right
  drive.turn_right(HALF_SPEED);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

  // Cruise at a constant speed
  drive.cruise(HALF_SPEED, true);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);

  // Test decceleration
  drive.deccelerate(ACC_MINIMAL, true);
  drive.get_params(&curr_speed, &curr_rpm, &state);
  Serial.println(curr_speed);
  Serial.println(curr_rpm);
  Serial.println(state);

  delay(1000);
}
