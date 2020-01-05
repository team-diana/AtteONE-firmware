#include "WiFi.h"
#include <PubSubClient.h>
#include "MotorControl.hpp"
#include "ProximitySensor.hpp"

//#define SERIAL_DEBUG

// MQTT Client Configuration and ROVER settings
const int   ATTE_ID = 1;
const char* MQTT_USER      = "AtteONE";
const char* SSID           = "DIANA";
const char* WIFI_PASSWORD  = "Starachowice";
const char* MQTT_BROKER_IP = "10.0.0.42";
const int   MQTT_PORT      = 1883;
/**********************************************************/

// TOPICS subscriptions
String ROVER_ROOT_TOPIC = String("atte") + String(ATTE_ID);
String MOVE_TOPIC = ROVER_ROOT_TOPIC + "/move";
String TANK_TOPIC = ROVER_ROOT_TOPIC + "/tank";
String ALIVE_TOPIC = ROVER_ROOT_TOPIC + "/alive";
String ENABLE_TOPIC = ROVER_ROOT_TOPIC + "/enable";

bool rover_enable = true;
unsigned int reconnection_counter = 0;

#define LED 5
bool led_status;
int led_ticks_normal = 3500;
int led_ticks_rover_disabled = 17000;

char buffer[4]={0};
char command;
char value;

/* Sensor's declarations */
long duration; //add
float distance; //add

WiFiClient esp_client;
PubSubClient mqtt_client(esp_client);

uint32_t alive_count = 0;

void setuWifi() {
    motorHardStop();
    WiFi.begin(SSID, WIFI_PASSWORD);
    // mqtt_client.connect(MQTT_USER);

    digitalWrite(LED, HIGH);
    Serial.println("Connecting to Wifi...");
    while(WiFi.status()!= WL_CONNECTED){
        Serial.print(".");
        // W of Wifi in morse
        digitalWrite(LED, LOW);
        delay(150);
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(300);
        digitalWrite(LED, HIGH);
        delay(100);
        digitalWrite(LED, LOW);
        delay(300);
        digitalWrite(LED, HIGH);
        delay(400);
    }
    Serial.println();

    Serial.println("Connected!");
    mqtt_client.setServer(MQTT_BROKER_IP, MQTT_PORT);
    mqtt_client.setCallback(mqttMessageCallback);

    reconnect();
}

void reconnect() {
    // Loop until we're reconnected
    while (!mqtt_client.connected()) {
        Serial.print("Attempting MQTT connection...");

        digitalWrite(LED, HIGH);
        // Attempt to connect
        if (mqtt_client.connect(MQTT_USER)) {
            Serial.println("connected");
            //Once connected, publish an announcement...
            if (reconnection_counter == 0) {
                mqtt_client.publish("atte1/status", "ready and operational");
            } else mqtt_client.publish("atte1/status", String("reconnected for the " + String(reconnection_counter) + " time").c_str());
            reconnection_counter++;
            // ... and resubscribe
            mqtt_client.subscribe(ENABLE_TOPIC.c_str());
            mqtt_client.subscribe(MOVE_TOPIC.c_str());
            mqtt_client.subscribe(TANK_TOPIC.c_str());
            mqtt_client.subscribe("atte0/#");
            Serial.println("Subscribed to MQTT topics:");
            Serial.println("atte0/#");
            Serial.println(ENABLE_TOPIC);
            Serial.println(MOVE_TOPIC);
            Serial.println(TANK_TOPIC);
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 3 seconds");
            // Wait 3 seconds before retrying

            // B of mqtt Broker in Morse
            digitalWrite(LED, LOW);
            delay(300);
            digitalWrite(LED, HIGH);
            delay(100);
            digitalWrite(LED, LOW);
            delay(150);
            digitalWrite(LED, HIGH);
            delay(100);
            digitalWrite(LED, LOW);
            delay(150);
            digitalWrite(LED, HIGH);
            delay(100);
            digitalWrite(LED, LOW);
            delay(150);
            digitalWrite(LED, HIGH);
            delay(100);
            delay(1950);
        }
    }
}

void mqttMessageCallback(char* topic, byte* b_payload, unsigned int length) 
{
    if (length > 50) return;
    char c_payload[51];
    for (int i=0; i<length; i++)
    {
    c_payload[i] = (char) b_payload[i];
    }
    
    c_payload[length] = '\0';

    String s_topic = String(topic);
    String s_payload = String(c_payload);

    #ifdef SERIAL_DEBUG
    Serial.print("Message arrived in topic: ");
    Serial.println(s_topic);
    Serial.println(s_payload);
    Serial.println(c_payload);

    Serial.print("Message:  ");
    for (int i = 0; i < length; i++) 
    {
    Serial.print((char)b_payload[i]);
    }
    Serial.println();
    #endif


    if (s_topic == ENABLE_TOPIC || s_topic == "atte0/enable" ) 
    {
        int en = 1;
        sscanf(c_payload, "%d", &en);
        rover_enable = (en != 0 ? true : false);
    }
    
         
      else if (s_topic == MOVE_TOPIC || s_topic == "atte0/move" ) 
      {
        if (rover_enable) 
        {
         float left_speed = 0, right_speed = 0;
         float norm_speed = 0, direction = 0;
         sscanf(c_payload, "%f %f", &norm_speed, &direction);
        
        distance=sensor(); //add
        Serial.print("Distance: "); //add
        Serial.println(distance); //add

         if (norm_speed <= 0.01 || distance < 10) //add
            {
             norm_speed = 0;
             motorSoftStop();
            } 
            else 
              {
               slipSteeringSaturationProfile(norm_speed, direction, &left_speed, &right_speed);
               driveMotor(left_speed, right_speed);
              }
        }
      }
      else if (s_topic == TANK_TOPIC || s_topic == "atte0/tank") 
        {
        if (rover_enable)
          {
           float tank_left = 0, tank_right = 0;
           sscanf(c_payload, "%f %f", &tank_left, &tank_right);
           driveMotor(tank_left, tank_right);
          }
        }
}

void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);
    motorInitAll();
    setupsensor(); //add
    pwmInit();
    setuWifi();
}

void loop() {
    // Check WiFi connection
    if (WiFi.status() != WL_CONNECTED) setuWifi();

    // Check if MQTT client is still connected
    if (!mqtt_client.connected()) {
        motorHardStop();
        reconnect();
    }
    mqtt_client.loop(); // Run main MQTT client event
    if (!rover_enable) motorHardStop();

    // Send alive message on MQTT and blink LED
    if ((alive_count % (rover_enable ? led_ticks_normal : led_ticks_rover_disabled)) == 0) {
        led_status = !led_status;
        digitalWrite(LED, led_status);
    }
    if ((alive_count % 40000) == 0) {
        mqtt_client.publish(ALIVE_TOPIC.c_str(), "alive");
    }
    alive_count++;
}
