#include <telemetry.pb.h>
#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Telemetry.h>

char ssid[] = "TP-LINK_2.4GHz_677647";    //  your network SSID (name) 
char pass[] = "98238316";   // your network password
const uint16_t port  = 10101;
const char* addr = "192.168.0.101";
WiFiClient  client;

Telemetry telemetry(1000);

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Setting up WIFI for SSID ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WIFI connection failed, reconnecting...");
    delay(500);
  }

  Serial.println("");
  Serial.print("WiFi connected, ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
//  Serial.print("connecting to ");
//  Serial.println(addr);
//
//  if (!client.connect(addr, port)) {
//    Serial.println("connection failed");
//    Serial.println("wait 5 sec to reconnect...");
//    delay(5000);
//    return;
//  }

  // put your main code here, to run repeatedly:
  bool upload_status = telemetry.uploadUltrasonic(10.2, 22.22, 33.33, client);
  if (upload_status){
    Serial.println("US Upload Successful");
  }
  else {
    Serial.println("US Upload Failed");
  }
  delay(5000);

}
