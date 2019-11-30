#include "mqtt.hpp"
char buffer[4]={0};
char command;
extern PubSubClient client;

void setup() {
  Serial.begin(115200);
  all_motorInit();
  pwm_Init();
  mqtt_wifi_init();
  client.subscribe("atte1/move");
}

void callback(char* topic, byte* payload, unsigned int length) {

  Serial.print("Message arrived in topic: ");
  Serial.println(topic);

  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  float val1;
  float val2;


  if (strcmp(topic, "atte1/move") == 0) {

    float left_speed;
    float right_speed;
    int segno1,segno2;
    float norm_speed,direction;

    sscanf((char*)payload,"%f%f",&norm_speed,&direction);

    slipSteeringSaturationProfile(norm_speed,direction,&left_speed,&right_speed);
    segno1    = left_speed/abs(left_speed);   //SX
    segno2    = right_speed/abs(right_speed);   //DX
    drive_Motor(segno1,segno2,abs((int) left_speed),abs((int) right_speed));
  }
  else if (strcmp(topic, "atte1/tank") == 0) {
  int val1,val2;
  sscanf((char*)payload,"%f%f",&val1,&val2);
  int segno1,segno2;
  segno1    = val1/abs(val1);   //SX
  segno2    = val2/abs(val2);   //DX
  if(WiFi.status()== WL_CONNECTED && client.connected()){
	     drive_Motor(segno1,segno2,abs(val1),abs(val2));
    }
    else {
      motorHardStop();
    }
}

  Serial.println();
  Serial.println("-----------------------");
}

void loop() {

  client.loop();
  if (Serial.available()>0) {
    Serial.readBytes(buffer,3);
    command     = buffer[0];
    value       = buffer[2];
  }

  switch (command) {
    case '0':
      motorSoftStop();
      Serial.println("Soft stop");
      break;
    default:
      Serial.println("Invalid command");
      break;
  }

}
