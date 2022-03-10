//Initializing Pin
const int ENCODER_PIN = 9; 
const int PWM_PIN = 11;
const int DIR_PIN = 10;
bool flag = HIGH;
int i_speed = 0;
int pulse_read = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT); //direction control PIN 10 with direction wire 
  pinMode(ENCODER_PIN, INPUT);
}

void loop() {
  int i = 0; 
  for(i=0; i<255; i++){
    analogWrite(PWM_PIN, i);

  // Spin at medium speed
  analogWrite(PWM_PIN, i);
//    Serial.print("Speed: ");
  Serial.print(i);
  Serial.print(",");

  delay(2500);

  // Encoder Read
  for(int j = 0;j<8;j++)  {
    pulse_read += pulseIn(9, HIGH, 500000); //SIGNAL OUTPUT PIN 9 with  white line,cycle = 2*i,1s = 1000000us，Signal cycle pulse number：27*2
  }
  pulse_read = pulse_read >> 3;
//  Serial.print(111111 / pulse_read); //speed   r/min  (60*1000000/(45*6*2*i))
  Serial.print(111111 / pulse_read);
   Serial.println("");
  pulse_read = 0;

  delay(2500);

  }

}
