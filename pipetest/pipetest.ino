#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
char ssid[] = "";    //  your network SSID (name) 
char pass[] = "";   // your network password
int status = WL_IDLE_STATUS;
WiFiClient  client;

unsigned long myChannelNumber = 1641314;  //put your channel number
const char * myWriteAPIKey = "P8VEPP035BU7SAFM";  //put your write API key
const char* addr = "https://enhma6gj0y3vv7v.m.pipedream.net"; //edit the host adress, ip address etc. 
int trigPin = 13;    // TRIG pin
int echoPin = 12;    // ECHO pin

float duration_us, distance_cm;
const int httpPort = 80;
void wifi1() {
   WiFi.begin(ssid, pass);
   WiFi.begin(ssid, pass);
   Serial.println("Connecting");
 while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
//  WiFiClient client;
}

void setup() {
  // begin serial port
  Serial.begin (115200);

  // configure the trigger pin to output mode
  pinMode(trigPin, OUTPUT);
  // configure the echo pin to input mode
  pinMode(echoPin, INPUT);
  wifi1();
}

void loop() {
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  
  StaticJsonDocument<200> doc;
  doc["distance"] = distance_cm;
  String postData;
  serializeJson(doc, postData);



  if (client.connect(addr, 80)){
    client.println("POST / HTTP/1.1");
    client.println("Host: enhma6gj0y3vv7v.m.pipedream.net");
    client.println("Cache-Control: no-cache");
    client.println("Connection: keep-alive");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);
    }
  while (client.connected())
  {
    while ( client.available() )
    {
      char str=client.read();
     Serial.println(str);
    }
    delay(1000);
    break;      
  }
  delay(500);
}
