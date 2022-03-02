// Motor Control Test

int i = 0;
unsigned long time = 0;
bool flag = HIGH;

void setup() {
  Serial.begin(115200);
  pinMode(10, OUTPUT); //direction control PIN 10 with direction wire 
  pinMode(11, OUTPUT); //PWM PIN 11  with PWM wire
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() - time > 5000)  {
    flag = !flag;
    digitalWrite(10, flag);
    time = millis();
  }
  if (Serial.available())  {
    analogWrite(11, Serial.parseInt());  //input speed (must be int)
    delay(200);
  }
  for(int j = 0;j<8;j++)  {
    i += pulseIn(9, HIGH, 500000); //SIGNAL OUTPUT PIN 9 with  white line,cycle = 2*i,1s = 1000000us，Signal cycle pulse number：27*2
  }
  i = i >> 3;
  Serial.print(111111 / i); //speed   r/min  (60*1000000/(45*6*2*i))
  Serial.println("  r/min");
  i = 0;
}
