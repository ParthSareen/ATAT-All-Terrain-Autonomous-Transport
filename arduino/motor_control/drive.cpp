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

  int _current_speed = 0; 
  int _mode = STOPPED; 
}

/**
* input parameter from rate enum, e.g. "low"
*
* Currently, accelerate only operates in the forward direction
*/
void Drive::accelerate(int rate){

	Serial.println("==> Accelerating");

	// Set direction pins so that robot moves forward
	_set_forward(_dir_pin_left, _dir_pin_right);

	for(int i=255; i>0; i--){
	analogWrite(_pwm_pin_left, i);
	analogWrite(_pwm_pin_right, i);
	Serial.print("Speed up: ");
	Serial.println(i);
	delay(5+50*(1-rate/6)); //vary scalar multiple to adjust minimum acceleration
	}
}

// ============== Helper Methods ==============

void _set_forward(int left_pin, int right_pin){
	digitalWrite(left_pin, 1);
  	digitalWrite(right_pin, 0);
}
void _set_backward(){
	digitalWrite(left_pin, 0);
  	digitalWrite(right_pin, 1);
}
void _set_right(){
	digitalWrite(left_pin, 0);
  	digitalWrite(right_pin, 0);
}
void _set_left(){
	digitalWrite(left_pin, 1);
  	digitalWrite(right_pin, 1);
}