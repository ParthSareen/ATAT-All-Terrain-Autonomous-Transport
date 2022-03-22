#include "Telemetry.h"
#include "Arduino.h"
#include "telemetry.pb.h"
#include "pb.h"
#include <pb_common.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <ESP8266WiFi.h>


Telemetry::Telemetry(int waitDelay, WiFiClient* client, char* addr, uint16_t port) {
    _waitDelay = waitDelay;
    _client = client;
    _addr = addr;
    _port = port;
    // Serial.println(_waitDelay, )
}

bool Telemetry::uploadUltrasonic(float us_front, float us_left, float us_back) {
      pb_TelemetryEvent ultrasonic_event = pb_TelemetryEvent_init_zero;

      ultrasonic_event.tel_cmd = pb_TelemetryEvent_Telemetry_Command_CMD_ULTRASONIC;
      ultrasonic_event.tel_us.us_front = us_front;
      ultrasonic_event.tel_us.us_left = us_left;
      ultrasonic_event.tel_us.us_back = us_back;
      ultrasonic_event.has_tel_us = true;
      return _uploadEvent(ultrasonic_event);

}

bool Telemetry::uploadImuAccel(float accel_x, float accel_y, float accel_z) {
    pb_TelemetryEvent imu_accel_event = pb_TelemetryEvent_init_zero;

    imu_accel_event.tel_cmd = pb_TelemetryEvent_Telemetry_Command_CMD_ACCELERATION;
    imu_accel_event.has_tel_acc = true;
    imu_accel_event.tel_acc.accel_x = accel_x;
    imu_accel_event.tel_acc.accel_y = accel_y;
    imu_accel_event.tel_acc.accel_z = accel_x;

    return _uploadEvent(imu_accel_event);
}

bool Telemetry::uploadImuGyro(float gyro_x, float gyro_y, float gyro_z) {
    pb_TelemetryEvent imu_gryo_event = pb_TelemetryEvent_init_zero;

    imu_gryo_event.tel_cmd = pb_TelemetryEvent_Telemetry_Command_CMD_GYRO;
    imu_gryo_event.has_tel_gyro = true;
    imu_gryo_event.tel_gyro.gyro_x = gyro_x;
    imu_gryo_event.tel_gyro.gyro_y = gyro_y;
    imu_gryo_event.tel_gyro.gyro_z = gyro_z;

    return _uploadEvent(imu_gryo_event);
}

bool Telemetry::uploadOrientation(int orientation) {
    if (orientation > 3 || orientation < 0) {
        return false;
    }
    
    pb_TelemetryEvent robot_orientation_event = pb_TelemetryEvent_init_zero;
    
    pb_TelemetryEvent_Orientation pb_orientation = static_cast<pb_TelemetryEvent_Orientation>(orientation);
    Serial.println(pb_orientation);

    robot_orientation_event.has_tel_orientation = true;
    robot_orientation_event.tel_cmd = pb_TelemetryEvent_Telemetry_Command_CMD_ORIENTATION;
    robot_orientation_event.tel_orientation.orientation = pb_orientation;

    return _uploadEvent(robot_orientation_event);
}

bool Telemetry::uploadShutdownStatus(bool shutdown_status) {
    pb_TelemetryEvent shutdown_status_event = pb_TelemetryEvent_init_zero;

    shutdown_status_event.improper_shutdown = shutdown_status;
    shutdown_status_event.tel_cmd = pb_TelemetryEvent_Telemetry_Command_CMD_SHUTDOWN;

    return _uploadEvent(shutdown_status_event);
}

bool Telemetry::uploadEncoder(float left_encoder, float right_encoder) {
    pb_TelemetryEvent encoder_event = pb_TelemetryEvent_init_zero;

    encoder_event.has_tel_enc = true;
    encoder_event.tel_cmd = pb_TelemetryEvent_Telemetry_Command_CMD_ENCODER;
    encoder_event.tel_enc.leftMotor = left_encoder;
    encoder_event.tel_enc.rightMotor = right_encoder;

    return _uploadEvent(encoder_event);
}

bool Telemetry::uploadLocation() {
    // pb_TelemetryEvent location_event = pb_TelemetryEvent_init_zero;

    // location_event.has_tel_loc = true;
    // location_event.tel_loc.cols = cols;
    // location_event.tel_loc.rows = rows;
    // TODO: Fix
    // location_event.tel_loc.data = data;
    return false;
    // return _uploadEvent(&location_event);
}

bool Telemetry::uploadSpeed(int speed) {
    if (speed > 3 || speed < 0) {
        return false;
    }
    
    pb_TelemetryEvent speed_event = pb_TelemetryEvent_init_zero;
    
    pb_TelemetryEvent_Motor_Speed pb_speed = static_cast<pb_TelemetryEvent_Motor_Speed>(speed);
    Serial.print("casted val ");
    Serial.println(pb_speed);
    speed_event.has_tel_motor_speed = true;
    speed_event.tel_cmd = pb_TelemetryEvent_Telemetry_Command_CMD_MOTOR_SPEED;
    speed_event.tel_motor_speed.motorSpeed = pb_speed;

    return _uploadEvent(speed_event);
    
}

bool Telemetry::_uploadEvent(pb_TelemetryEvent event){
    // TODO: Refactor size to private const
    // TODO: Make this non-blocking code, fault tolerant
    uint8_t buffer[128];
    uint8_t readBuffer[128];
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));

    if (!pb_encode(&stream, pb_TelemetryEvent_fields, &event)){
        Serial.println("failed to encode proto");
        Serial.println(PB_GET_ERROR(&stream));
        return false;
    }

    bool connect_status = _client->connect(_addr, _port);
    uint32_t retry_count = 0;
    const uint32_t MAX_RETRY = 3;
    // TODO check locking
    while (connect_status == false and retry_count < MAX_RETRY) {
        Serial.println("Connect failed");
        bool connect_status = _client->connect(_addr, _port);
        retry_count++;
        // delay(15);
    }

    // Exit if max reached
    // if (retry_count == MAX_RETRY){
    //     Serial.println("Max retry reached");
    //     return false;
    // }
    
    _client->write(buffer, stream.bytes_written);
    // client.flush();
    _client->stop(30);
    // // TODO: refactor to not being blocked and having timeout instead
    // while (_client->available()) {}
    return true;

}

