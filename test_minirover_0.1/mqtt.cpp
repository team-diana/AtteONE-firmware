#include "mqtt.hpp"
#include "MotorControl.h"
#include "Wifi.h"
#include <PubSubClient.h>


void
mqtt_wifi_init() {
        
        client.connect();
        PubSubClient client(espClient);
        WiFi.begin(ssid,password);
        
        while(WiFi.status()!= WL_CONNECTED){
                delay(500);
                Serial.print("Connecting to Wifi...");
        }
        
        Serial.println("Connected!");
        client.setServer(mqttServer, mqttPort);
        client.setCallback(callback);
        
        while(!client.connected()){
                Serial.println("Connecting to MQTT...");
                if(client.connect("Client",mqttUser,mqttPassword)) {
                       Serial.println("Connected");
                }
                else {
                        Serial.println("failed with state");
                        Serial.println(client.state()); 
                        delay(2000);
                }
        }
}


void 
callback(char* topic, byte* payload, unsigned int length) {
        int n_Atte              = get_Atte(topic);              //returns Atten number, where n is the number of the mini-rover        
        Serial.print("Message arrived in topic: ");
        Serial.println(topic);
        
        Serial.print("Message:");
        for (int i = 0; i < length; i++) {
               Serial.print((char)payload[i]);
        }
                                                                //around here, it should access into an array where are stored
                                                                //all the infos about the Atte mini-rovers


        Serial.println();
        Serial.println("-----------------------");
        
}

int
get_Atte(char* topic) {
        int i                   = 0;
        while (!isdigit(topic[i]));
        return topic[i];
}