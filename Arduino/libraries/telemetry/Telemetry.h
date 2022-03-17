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
        Telemetry(int wait_delay, WiFiClient* client, char* addr, uint16_t port);

        // I miss gRPC
        // Upload Ultrasonic data to server
        bool uploadUltrasonic(float us_front, float us_left, float us_back);
        bool uploadImuAccel(float accel_x, float accel_y, float accel_z);
        bool uploadImuGyro(float gyro_x, float gyro_y, float gyro_z);
        
        bool uploadEncoder(float left_encoder, float right_encoder);
        bool uploadLocation(uint32_t cols, uint32_t rows, int32_t [][]data);
        bool uploadSpeed(int speed);
        // Make sure enum mappings correlate with protobuf
        bool uploadOrientation(int orientation);
        bool uploadShutdownStatus(bool shutdown_status);

        bool getBackupData();

    private:
        int _waitDelay;
        bool _uploadEvent(pb_TelemetryEvent event);

        uint16_t _port  = 10101;
        // char* _addr = "192.168.0.101";
        char* _addr = "172.20.10.7";

        WiFiClient* _client;

};
#endif
