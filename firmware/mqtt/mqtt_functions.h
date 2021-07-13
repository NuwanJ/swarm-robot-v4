#pragma once

#include "features.h"
#include "config/config.h"
#include "modules/motors/motors.h"
#include "modules/memory/memory.h"
#include "modules/gpio/gpio.h"

#include "mqtt/mqtt_global_variables.h"
#include "mqtt/mqtt_robot/mqtt_robot.h"

void beginMQTT();

void mqtt_wait(uint8_t *lock);
void enter_critical();
void exit_critical();

void subscribeDefault();

void mqtt_subscribe(char* str);
int mqtt_publish(char* str1, char* str2, boolean retained);
void mqtt_onMessageArrived(char* topic, byte* message, unsigned int length);

void mqtt_handle();
void mqtt_reconnect();

void mqtt_delay(int time_in_ms);
void mqtt_log(String message);
