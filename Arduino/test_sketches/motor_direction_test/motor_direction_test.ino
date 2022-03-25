// Test which direction the motor spins based on GPIO High or Low

/*
Results: 

High ==> CW
LOW ==> CCW

*/

//Initializing Pin
const int PWM_PIN = 11;
const int DIR_PIN = 10;
bool flag = HIGH;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT); //direction control PIN 10 with direction wire 

}

void loop() {

  // Alternate direction
  flag = flag ^ HIGH;
  digitalWrite(DIR_PIN, flag);
  Serial.print("Direction: ");
  Serial.println(flag);

  // Spin at medium speed
  analogWrite(PWM_PIN, 128);

  delay(5000);

}
