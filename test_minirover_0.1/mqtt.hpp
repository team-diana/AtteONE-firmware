#ifndef MQTT
#define MQTT
//#include <Arduino.h>


const char*     ssid            = "DIANA";
const char*     pass            = "Starachowice";
const char*     Ip              = "10.0.0.42";
const char*     mqttUser        = "Motor_client";

//const char* mqttPassword        = ""; //optional

const int       mqttPort        = 1883;

WiFiClient      espClient;


void mqtt_wifi_init();
int get_Atte();
void callback(char* topic, byte* payload, unsigned int length);

#endif
