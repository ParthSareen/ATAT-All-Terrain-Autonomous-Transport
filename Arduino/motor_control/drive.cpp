#include "drive.h"

// ============== Public Methods ==============

// Left right orientation based on looking at the robot from the front position
Drive::Drive(int pwm_pin_left, int pwm_pin_right, int dir_pin_left, int dir_pin_right, int encoder_pin_left, int encoder_pin_right){
  pinMode(pwm_pin_left, OUTPUT);
  pinMode(pwm_pin_right, OUTPUT);
  pinMode(dir_pin_left, OUTPUT);
  pinMode(dir_pin_right, OUTPUT);
  pinMode(encoder_pin_left, INPUT);
  pinMode(encoder_pin_right, INPUT);

  _pwm_pin_left = pwm_pin_left;
  _pwm_pin_right = pwm_pin_right;
  _dir_pin_left = dir_pin_left;
  _dir_pin_right = dir_pin_right;
  _encoder_pin_left = encoder_pin_left;
  _encoder_pin_right = encoder_pin_right;

  // Initialize
  _current_speed = 0; 
  _current_rpm = 0;
  _mode = STOPPED; 

  // Ensure motor is initially stopped
  analogWrite(_pwm_pin_left, 254);
  analogWrite(_pwm_pin_right, 254);
}

/**
* input parameter 1 from accel_rate enum, e.g. "ACC_LOW"
* input parameter 2 optional, defaults to true (forward)
*
* Currently, accelerate only operates in the forward direction
* Accelerates to maximun speed from the current speed
*/
void Drive::accelerate(int rate, bool fwd = true){

	Serial.println("==> Accelerating");

	// Set robot state
	_mode = ACCELERATING; 

	/// Set direction of motors
  fwd ? _set_forward() : _set_backward();

	// Get current speed as starting point
	int curr_speed_i = convert_speed_to_i(_current_speed); 

	for(int i=curr_speed_i; i>0; i--){

		analogWrite(_pwm_pin_left, i);
		analogWrite(_pwm_pin_right, i);
		Serial.print("Speed up: ");
		Serial.println(i);

		// Track new current speeds
		_current_speed = convert_i_to_speed(i); 
		_current_rpm = convert_i_to_rpm(i); 

		delay(5+50*(1-rate/6)); //vary scalar multiple to adjust minimum acceleration
	}

	// Set new speed 
	_current_speed = MAX_SPEED; 
	_current_rpm = MAX_RPM; 
}

/**
* input parameter from accel_rate enum, e.g. "ACC_LOW"
* input parameter 2 optional, defaults to true (forward)
* 
* Doesn't bring robot to a complete speed, only to a minimun speed from current position
*/
void Drive::deccelerate(int rate, bool fwd = true){

	Serial.println("==> Deccelerating");

	// Set robot state
	_mode = DECELERATING;

  // Set direction of motors
  fwd ? _set_forward() : _set_backward();
  
	// Get current speed as starting point
	int curr_speed_i = convert_speed_to_i(_current_speed); 

	Serial.println(curr_speed_i);

	for(int i=curr_speed_i; i<250; i++){

		analogWrite(_pwm_pin_left, i);
		analogWrite(_pwm_pin_right, i);
		Serial.print("Slow Down: ");
		Serial.println(i);

		_current_speed = convert_i_to_speed(i); 
		_current_rpm = convert_i_to_rpm(i);

		delay(5+50*(1-rate/6)); //vary scalar multiple to adjust minimum acceleration
	}

	// Set new speed 
	_current_speed = convert_i_to_speed(250); 
	_current_rpm = convert_i_to_rpm(250); 
}

/**
* input parameter optional; defaults to max speed
* 
* Spins clockwise indefinitely
*/
void Drive::turn_right(float speed = MAX_SPEED){

  // Set robot state
  _mode = TURNING;

  int turn_speed_i = convert_speed_to_i(speed); 

  // Command robot to turn
  _set_right();
  analogWrite(_pwm_pin_left, turn_speed_i);
  analogWrite(_pwm_pin_right, turn_speed_i);
  Serial.print("Turn right at ");
  Serial.println(turn_speed_i);

  // Update speed parameter
  _current_speed = turn_speed_i;
  _current_rpm = convert_speed_to_rpm(speed);
}

/**
* input parameter optional; defaults to max speed
* 
* Spins counterclockwise indefinitely
*/
void Drive::turn_left(float speed = MAX_SPEED){

  // Set robot state
  _mode = TURNING;

  int turn_speed_i = convert_speed_to_i(speed); 

  // Command robot to turn
  _set_left();
  analogWrite(_pwm_pin_left, turn_speed_i);
  analogWrite(_pwm_pin_right, turn_speed_i);
  Serial.print("Turn left at ");
  Serial.println(turn_speed_i);

  // Update speed parameter
  _current_speed = turn_speed_i;
  _current_rpm = convert_speed_to_rpm(speed);

}

void Drive::estop(){

  // Set robot state
  _mode = STOPPED;

  // Set PWM to stopped
  analogWrite(_pwm_pin_left, 254);
  analogWrite(_pwm_pin_right, 254);
  Serial.println("Stop");

  // Update speed parameter
  _current_speed = 0;
  _current_rpm = 0;

}

/*
 * input parameters optional; defaults to MAX_SPEED; true (forward)
 */
void Drive::cruise(float speed = MAX_SPEED, bool fwd = true){
  
  // Set robot state
  _mode = CRUISING;
  
  int cruise_speed_i = convert_speed_to_i(speed); 
  fwd ? _set_forward() : _set_backward();
  
  analogWrite(_pwm_pin_left, cruise_speed_i);
  analogWrite(_pwm_pin_right, cruise_speed_i);
  Serial.println("Cruise");

  // Update speed parameter
  _current_speed = speed;
  _current_rpm = convert_speed_to_rpm(speed);

}

/**
* input parameter 1 duration of encoder reading
* input parameter 2 accumulated speed
*
* returns the distance that's been travelled
*/
float * Drive::encoderReading(unsigned long *lastTime, float speed = MAX_SPEED, float encoderReadings[2]){
  float radius = 0.2075;
  int pulse_read_left = 0;
  int pulse_read_right = 0;
  float distance_left = 0;
  float distance_right = 0;
  float sim_rpm_left = 0;
  float sim_rpm_right = 0;
  float measured_rpm_left;
  float measured_rpm_right; 
  unsigned long currentTime = 0;
  float totalSeconds = 0;

  delay(2500);

  //Reads in the pulses and bit manipulations
  for(int j = 0;j<8;j++)  {
    pulse_read_right += pulseIn(_encoder_pin_right, HIGH, 500000); //SIGNAL OUTPUT PIN 9 with  white line,cycle = 2*i,1s = 1000000usï¼ŒSignal cycle pulse numberï¼š27*2
  }
  
  for(int j = 0;j<8;j++)  {
    pulse_read_left += pulseIn(_encoder_pin_left, HIGH, 500000); //SIGNAL OUTPUT PIN 9 with  white line,cycle = 2*i,1s = 1000000usï¼ŒSignal cycle pulse numberï¼š27*2
  }

  pulse_read_left = pulse_read_left >> 3;
  pulse_read_right = pulse_read_right >> 3;

  //Calculating simulated and measured 
  sim_rpm_left = ((-0.5795 * exp(0.02228*(111111/pulse_read_left))) + 156)/60.00;
  sim_rpm_right = ((-0.5795 * exp(0.02228*(111111/pulse_read_right))) + 156)/60.00;  
  measured_rpm_left = ((111111.000 / pulse_read_left) / 60.000);
  measured_rpm_right = ((111111.000 / pulse_read_right) / 60.000);
  
  //Time with regards to the total minutes since the last read
  currentTime = millis();
  totalSeconds = ((currentTime - *lastTime) / 1000.000);
  *lastTime = currentTime;

  if (abs(sim_rpm_left - measured_rpm_left) > 5){
    distance_left = totalSeconds * sim_rpm_left * (2 * PI * radius);
  }else{
    distance_left = totalSeconds * measured_rpm_left * (2 * PI * radius);
  }

 if (abs(sim_rpm_right - measured_rpm_right) > 5){
    distance_right = totalSeconds * sim_rpm_right * (2 * PI * radius);
  }else{
    distance_right = totalSeconds * measured_rpm_right * (2 * PI * radius);
  }

  encoderReadings[0] = distance_left;
  encoderReadings[1] = distance_right;

  return;
}

/*
 * Get values of private members of the class
 */
void Drive::get_params(float* curr_speed, int* curr_rpm, int* mode){
	*curr_speed = _current_speed;  
	*curr_rpm = _current_rpm;
	*mode = _mode;
}

// ============== Helper Methods ==============

// Note: 
// COUNTERCLOCKWISE = 0,
// CLOCKWISE = 1

// Forward: 
// Right => CCW
// Left => CW

void Drive::_set_forward(){
	digitalWrite(_dir_pin_left, CLOCKWISE);
  digitalWrite(_dir_pin_right, COUNTERCLOCKWISE);
}
void Drive::_set_backward(){
	digitalWrite(_dir_pin_left, COUNTERCLOCKWISE);
  digitalWrite(_dir_pin_right, CLOCKWISE);
}
void Drive::_set_right(){
	digitalWrite(_dir_pin_left, COUNTERCLOCKWISE);
  digitalWrite(_dir_pin_right, COUNTERCLOCKWISE);
}
void Drive::_set_left(){
	digitalWrite(_dir_pin_left, CLOCKWISE);
  digitalWrite(_dir_pin_right, CLOCKWISE);
}

// Note: 
// i = 0 => Max speed
// i = 255 => No speed
// V_max = 0.333m/s
// omega_max = 16.65 rad/s

float convert_i_to_speed(int i){
	return ((255 - i)/255)*(0.333);
}

int convert_i_to_rpm(int i){
	return ((255 - i)/255)*(159);
}

int convert_speed_to_i(float speed){
	return 255 - ((255*speed)/0.333);
}

int convert_speed_to_rpm(float speed){
  return convert_i_to_rpm(convert_speed_to_i(speed));
}

int convert_rpm_to_i(int rpm){
	return 255 - ((255*rpm)/159);
}