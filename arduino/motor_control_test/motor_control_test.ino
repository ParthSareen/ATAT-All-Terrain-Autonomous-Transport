//Initializing Pin
const int PWM_PIN = 11;
const int DIR_PIN = 10;
bool flag = HIGH;
int i_speed = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT); //direction control PIN 10 with direction wire 
  
  digitalWrite(DIR_PIN, flag);
  Serial.print("Direction: ");
  Serial.println(flag);

}

void loop() {

  int i_speed = Serial.parseInt(); 

  // Spin at medium speed
  analogWrite(PWM_PIN, i_speed);
    Serial.print("Speed: ");
  Serial.println(i_speed);

  delay(5000);

}
