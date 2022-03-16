//Initializing Pin
const int ENCODER_PIN = 9; 
const int PWM_PIN = 11;
const int DIR_PIN = 10;
bool flag = HIGH;
int i_speed = 0;
int pulse_read = 0;
int set = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT); //direction control PIN 10 with direction wire 
  pinMode(ENCODER_PIN, INPUT);
}

void loop() {

  while(set == 0){
    Serial.println("==> Waiting ");
    delay(50);
    set = Serial.parseInt();
    Serial.print(set);
    if(set != 0){
      break;
    }
  }

  // Spin at medium speed
  analogWrite(PWM_PIN, i_speed);
  Serial.print("Speed: ");
  Serial.println(i_speed);

  // Encoder Read
  for(int j = 0;j<8;j++)  {
    pulse_read += pulseIn(9, HIGH, 500000); //SIGNAL OUTPUT PIN 9 with  white line,cycle = 2*i,1s = 1000000us，Signal cycle pulse number：27*2
  }
  pulse_read = pulse_read >> 3;
  Serial.print(111111 / pulse_read); //speed   r/min  (60*1000000/(45*6*2*i))
  Serial.println("  r/min");
  pulse_read = 0;

  delay(50);

}
