#pragma once

// Enable or disable serial monitor for libraries, not impelmented
#define ENABLE_SERIAL_COMMUNICATION 1

// Enable neopixel indications for important event
#define NEOPIXEL_INDICATIONS
/*------------------------------------
Magenta:    WiFi connected successfully
Red:        Attempt MQTT reconnect
Green:      MQTT reconnect attempt success
Blue:       -
/ -----------------------------------*/

// Enable internal EEPROM memory, REQUIRED
#define ENABLE_MEMORY

// -----------------------------------
// Enable robot motors
#define ENABLE_MOTORS

#ifdef ENABLE_MOTORS
// Select one of the below 2 options
//#define DRIVE_SERVO // not fully tested yet
#define DRIVE_PWM

#define WHEEL_ENCODER
#endif
// -----------------------------------

// Enable NeoPixel Ring
#define ENABLE_NEOPIXEL_RING

// Enable VL53LX0 ToF Sensor
#define ENABLE_DISTANCE_SENSOR

// Enable GP2Y0A21YK0F analog distance sensor or VL53LX0 laser ranging sensor
// #define DISTANCE_GP2Y0A21YK0F
#define DISTANCE_VL53LX0

// Enable TCS34725 Color sensor
#define ENABLE_COLOR_SENSOR

// Enable GY-511 compass + accelerometer
#define ENABLE_COMPASS_SENSOR

// Enable PCF8564 I2C port expander
//#define ENABLE_EXTERNAL_PORT

// Enable IR Transmiter and receiver
//#define ENABLE_INFARED

// Enable or disable OTA
//    (On the air upload via WiFi)
//#define ENABLE_OTA_UPLOAD

// Enable Wifi monitor, not fully tested yet
//#define ENABLE_WIFI_MONITOR

// Eable WiFi client, not fully tested yet
//#define ENABLE_WIFI_CLIENT

// Enable ESP Now protocol not fully tested yet
//    (No WiFi allowed in this mode)
// #define ENABLE_ESPNOW

// SSD1306 optional display module
//    (specially for debugging purposes)
// #define ENABLE_SSD1306_DISPLAY

// MQTT protocol for communication
#define ENABLE_MQTT
#define ENABLE_WIFI

// Enable Virtual Reality support
#define ENABLE_VIRT_READINGS

// Can use 'ONLY ONE' of the following algorithms
// #define ALGO_COLOR_RIPPLE
// #define ALGO_MOVE_ROBOT
// #define ALGO_DISCOVER_COLOR
#define ALGO_COMMUNICATION

// Dependecy check for motors
#if (defined(ENABLE_MOTORS) && !(defined(DRIVE_SERVO) || defined(DRIVE_PWM)))
#error "DEPENDENCY ERROR: Need to be defined either DRIVE_SERVO or DRIVE_PWM in features.h"
#endif

// Dependecy check for distance sensor
#if (defined(ENABLE_DISTANCE_SENSOR) && !(defined(DISTANCE_GP2Y0A21YK0F) || defined(DISTANCE_VL53LX0)))
#error "DEPENDENCY ERROR: Need to enable either GP2Y0A21YK0F or VL53LX0 sensor in features.h"
#endif
#if (defined(ENABLE_DISTANCE_SENSOR) && defined(DISTANCE_GP2Y0A21YK0F) && defined(DISTANCE_VL53LX0))
#error "DEPENDENCY ERROR: You can't enable both GP2Y0A21YK0F and VL53LX0 sensors at once in features.h"
#endif

// Dependency check for MQTT
#if (defined(ENABLE_MQTT) && !(defined(ENABLE_WIFI)))
#error "DEPENDENCY ERROR: Need to enable WiFi by uncommenting #define ENABLE_WIFI in features.h to enable the MQTT"
#endif
