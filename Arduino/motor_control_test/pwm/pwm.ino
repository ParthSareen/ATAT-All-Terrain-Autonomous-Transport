// PWM Test

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
  int i=0;

  // Alternate direction
  flag = flag ^ HIGH;
  digitalWrite(DIR_PIN, flag);

   //Speed up
  for(i=255; i>0; i--){
    analogWrite(PWM_PIN, i);
    Serial.print("Speed up: ");
    Serial.println(i);
    delay(10);
  }
  
  delay(250);
    
  //Slowdown
  for(i=1; i<255; i++){
    analogWrite(PWM_PIN, i);
    Serial.print("Speed down: ");
    Serial.println(i);
    delay(10);
  }

}
