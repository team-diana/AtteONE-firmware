#ifndef MQTT
#define MQTT
#include "WiFi.h"
#include <PubSubClient.h>
#include "MotorControl.h"






void mqtt_wifi_init();
int get_Atte(char *topic);
void callback(char* topic, byte* payload, unsigned int length);

#endif
