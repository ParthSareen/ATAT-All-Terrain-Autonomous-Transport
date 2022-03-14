#include "Telemetry.h"
#include "Arduino.h"
#include "telemetry.pb.h"
#include "pb.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <ESP8266WiFi.h>


Telemetry::Telemetry(int waitDelay) {
    _waitDelay = waitDelay;
    // _client = client;
}

bool Telemetry::uploadUltrasonic(float us_front, float us_left, float us_back, WiFiClient client) {
      pb_TelemetryEvent event = pb_TelemetryEvent_init_zero;

      event.tel_cmd = pb_TelemetryEvent_Telemetry_Command_CMD_ULTRASONIC;
      event.tel_us.us_front = us_front;
      event.tel_us.us_left = us_left;
      event.tel_us.us_back = us_back;
      event.has_tel_us = true;
      return _uploadEvent(event, client);

}

bool Telemetry::_uploadEvent(pb_TelemetryEvent event, WiFiClient client){
    pb_TelemetryEvent tel = pb_TelemetryEvent_init_zero;

    // TODO: Refactor size to private const
    uint8_t buffer[128];
    uint8_t readBuffer[128];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    // Serial.print("left");
    // Serial.println(event.tel_us.us_left);
    // bool encode_status = pb_encode(&stream, pb_TelemetryEvent_fields, &event);

    if (!pb_encode(&stream, pb_TelemetryEvent_fields, &event)){
        Serial.println("failed to encode proto");
        Serial.println(PB_GET_ERROR(&stream));
        return false;
    }

    bool connect_status = client.connect(_ADDR, _PORT);
    uint32_t retry_count = 0;
    const uint32_t MAX_RETRY = 3;
    // TODO check locking
    while (connect_status == false and retry_count < MAX_RETRY) {
        Serial.println("Connect failed");
        retry_count++;
        delay(100);
    }

    // Exit if max reached
    if (retry_count == MAX_RETRY){
        Serial.println("Max retry reached");
        return false;
    }
    client.write(buffer, stream.bytes_written);
    // client.flush();
    client.stop();
    delay(1000);
    // // TODO: refactor to not being blocked and having timeout instead
    // while (_client->available()) {}
    return true;

}

