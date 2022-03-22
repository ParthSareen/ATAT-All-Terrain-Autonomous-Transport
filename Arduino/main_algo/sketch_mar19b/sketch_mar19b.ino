#include <drive.h>

#define PWM_PIN_LEFT D3
#define PWM_PIN_RIGHT D6
#define DIR_PIN_LEFT D4
#define DIR_PIN_RIGHT D7

Drive motor_control(PWM_PIN_LEFT, PWM_PIN_RIGHT, DIR_PIN_LEFT, DIR_PIN_RIGHT); 

void setup() {
  Serial.begin(9600);

}

void loop() {
  Serial.print("Running Main");
  delay(5000);
  motor_control.cruise(MAX_SPEED, true);
  delay(60000);
  motor_control.estop(); 
}
