#include "mqtt.hpp"
WiFiClient      espClient;

PubSubClient client(espClient);
        

const char*     ssid            = "Iphone di Vincenzo";
const char*     pass            = "miamamma2";
const char*     Ip              = "192.168.43.223";
const char*     mqttUser        = "Motor_client";

//const char* mqttPassword        = ""; //optional

const int       mqttPort        = 1883;

void
mqtt_wifi_init() {
        WiFi.begin(ssid,pass);
        client.connect("Broker");
        

        while(WiFi.status()!= WL_CONNECTED){
                delay(500);
                Serial.print("Connecting to Wifi...");
        }

        Serial.println("Connected!");
        client.setServer(Ip, mqttPort);
        client.setCallback(callback);

        while(!client.connected()){
                Serial.println("Connecting to MQTT...");
                if(client.connect("Client")) {
                       Serial.println("Connected");
                }
                else {
                        Serial.println("failed with state");
                        Serial.println(client.state());
                        delay(2000);
                }
        }
        client.publish("ciao","ciao");
  
}





int
get_Atte(char* topic) {
        int i                   = 0;
        while (!isdigit(topic[i]));
        return topic[i];
}
