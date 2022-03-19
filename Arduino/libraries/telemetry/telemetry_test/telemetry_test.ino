#include <telemetry.pb.h>
#include <ESP8266WiFi.h>
//#include <ESP8266HTTPClient.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Telemetry.h>

char ssid[] = "TP-LINK_2.4GHz_677647";    //  your network SSID (name) 
//char ssid[] = "TP-LINK_5GHz_677648";    //  your network SSID (name) 
//char ssid[] = "k";

//char ssid[] = "iPhone";
char pass[] = "98238316";   // your network password
//char pass[] = "2FEE5A9E144A";
//char pass[] = "6cqvsnmh779pt";
uint16_t port  = 10101;
//char* addr = "172.20.10.7";
char* addr = "192.168.0.101";
WiFiClient  client;

Telemetry telemetry(1000, &client, addr, port);

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

  Serial.println("Running Telemetry Tests");
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

  unsigned long start = micros();
  Serial.print("Ultrasonic Test Status: ");
  bool ultrasonic_test = telemetry.uploadUltrasonic(20, 10, 2.3);
  Serial.println(ultrasonic_test);

  Serial.print("IMU Acceleration Test Status: ");
  bool imu_test_acc = telemetry.uploadImuAccel(1, 1.1, 12);
  Serial.println(imu_test_acc);

  Serial.print("IMU Gyro Test Status: ");
  bool imu_test_gyro = telemetry.uploadImuGyro(0, 2, 4);
  Serial.println(imu_test_gyro);

  Serial.print("Orientation Test Status: ");
  bool robot_orientation_test = telemetry.uploadOrientation(3);
  Serial.println(robot_orientation_test);
  unsigned long end = micros();
  unsigned long delta = end - start;
  Serial.print("Upload of all data took: ");
  Serial.println(delta);
  delay(5000);

}
