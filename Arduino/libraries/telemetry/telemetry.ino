#include <telemetry.pb.h>

#include <ESP8266WiFi.h>
#include <pb.h>
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>

char ssid[] = "anavacadothanks";    //  your network SSID (name) 
char pass[] = "";   // your network password
const uint16_t port  = 10101;
const char* addr = "192.168.0.100";
WiFiClient  client;


// setup WIFI and sensor
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
  Serial.print("connecting to ");
  Serial.println(addr);

  if (!client.connect(addr, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec to reconnect...");
    delay(5000);
    return;
  }

    
  pb_TelemetryEvent event = pb_TelemetryEvent_init_zero;
  event.timestamp = 24;
  event.data = 1.69;
  sendTemp(event);
 
  delay(5000);
}

void sendTemp(pb_TelemetryEvent e) {
  uint8_t buffer[128];
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  
  if (!pb_encode(&stream, pb_TelemetryEvent_fields, &e)){
    Serial.println("failed to encode temp proto");
    Serial.println(PB_GET_ERROR(&stream));
    return;
  }
  
  Serial.print("sending event...");
//  Serial.println(e.tempCel);
  client.write(buffer, stream.bytes_written);
}
