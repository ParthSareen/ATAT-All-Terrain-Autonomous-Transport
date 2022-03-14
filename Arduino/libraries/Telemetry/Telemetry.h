#ifndef Telemetry_h
#define Telemetry_h

#include "telemetry.pb.h"
#include "pb.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include "Arduino.h"
#include <ESP8266WiFi.h>


class Telemetry {
    public:
        Telemetry(int wait_delay);

        // I miss gRPC
        // Upload Ultrasonic data to server
        bool uploadUltrasonic(float us_front, float us_left, float us_back, WiFiClient client);
        bool uploadImuAccel();
        bool uploadImuGyro();
        
        bool uploadEncoder();
        bool uploadLocation();
        bool uploadSpeed();
        bool uploadOrientation();
        bool uploadShutdownStatus();

    private:
        int _waitDelay;
        bool _uploadEvent(pb_TelemetryEvent event, WiFiClient client);

        // Might be moved to arduino side for setup
        const char* _SSID = "TP-LINK_2.4GHz_677647";
        const char* _PASS = "98238316";
        const uint16_t _PORT  = 10101;
        const char* _ADDR = "192.168.0.101";

        WiFiClient* _client;

};
#endif