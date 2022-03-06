#include "drive.h"

// ============== Public Methods ==============

Drive::Drive(int pwm_pin_left, int pwm_pin_right, int dir_pin_left, int dir_pin_right){
  pinMode(pwm_pin_left, OUTPUT);
  pinMode(pwm_pin_right, OUTPUT);
  pinMode(dir_pin_left, OUTPUT);
  pinMode(dir_pin_right, OUTPUT);

  _pwm_pin_left = pwm_pin_left;
  _pwm_pin_right = pwm_pin_right;
  _dir_pin_left = dir_pin_left;
  _dir_pin_right = dir_pin_right;

  float _current_speed = 0; 
  int _current_rpm = 0;
  int _mode = STOPPED; 
}

/**
* input parameter from rate enum, e.g. "low"
*
* Currently, accelerate only operates in the forward direction
* Accelerates to maximun speed from the current speed
*/
void Drive::accelerate(int rate){

	Serial.println("==> Accelerating");

	// Set robot state
	_mode = ACCELERATING; 

	// Set direction pins so that robot moves forward
	_set_forward(_dir_pin_left, _dir_pin_right);

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
* input parameter from rate enum, e.g. "low"
* 
* Doesn't bring robot to a complete speed, only to a minimun speed from current position
*/
void Drive::deccelerate(int rate){

	Serial.println("==> Deccelerating");

	// Set robot state
	_mode = DECELERATING;

	// Get current speed as starting point
	int curr_speed_i = convert_speed_to_i(_current_speed); 

	for(int i=curr_speed_i; i<240; i++){

		analogWrite(_pwm_pin_left, i);
		analogWrite(_pwm_pin_right, i);
		Serial.print("Slow Down: ");
		Serial.println(i);

		_current_speed = convert_i_to_speed(i); 
		_current_rpm = convert_i_to_rpm(i);

		delay(5+50*(1-rate/6)); //vary scalar multiple to adjust minimum acceleration
	}

	// Set new speed 
	_current_speed = convert_i_to_speed(240); 
	_current_rpm = convert_i_to_rpm(240); 
}

/**
* input parameter optional; defaults to max speed
* 
* Spins clockwise indefinitely
*/
void Drive::turn_right(int speed = MAX_SPEED){

  // Set robot state
  _mode = TURNING;

  int turn_speed_i = convert_speed_to_i(speed); 

  _set_right();
  
  analogWrite(_pwm_pin_left, turn_speed_i);
  analogWrite(_pwm_pin_right, turn_speed_i);
  Serial.print("Turn right at");
  Serial.println(turn_speed_i);
  
  _current_speed = turn_speed_i;
}

/**
* input parameter optional; defaults to max speed
* 
* Spins counterclockwise indefinitely
*/
void Drive::turn_left(int speed = MAX_SPEED){

  // Set robot state
  _mode = TURNING;

  int turn_speed_i = convert_speed_to_i(speed); 

  _set_left();
  
  analogWrite(_pwm_pin_left, turn_speed_i);
  analogWrite(_pwm_pin_right, turn_speed_i);
  Serial.print("Turn left at");
  Serial.println(turn_speed_i);
  
  _current_speed = turn_speed_i;
}


// ============== Helper Methods ==============

// Note: 
// COUNTERCLOCKWISE = 0,
// CLOCKWISE = 1

// Forward: 
// Right => CCW
// Left => CW

void _set_forward(int left_pin, int right_pin){
	digitalWrite(left_pin, CLOCKWISE);
  	digitalWrite(right_pin, COUNTERCLOCKWISE);
}
void _set_backward(){
	digitalWrite(left_pin, COUNTERCLOCKWISE);
  	digitalWrite(right_pin, CLOCKWISE);
}
void _set_right(){
	digitalWrite(left_pin, COUNTERCLOCKWISE);
  	digitalWrite(right_pin, COUNTERCLOCKWISE);
}
void _set_left(){
	digitalWrite(left_pin, CLOCKWISE);
  	digitalWrite(right_pin, CLOCKWISE);
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

int convert_rpm_to_i(int rpm){
	return 255 - ((255*rpm)/159);
}
