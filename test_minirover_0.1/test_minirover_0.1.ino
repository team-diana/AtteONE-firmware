#include "MotorControl.h"
#define offset 48
char buffer[4]={0};
char command;
int value;

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  all_motorInit();           //Initialise motors pin
  pwm_Init();
}
void loop() {

  if (Serial.available()>0) {
    Serial.readBytes(buffer,3);
    command     = buffer[0];
    value       = buffer[2];
  }

  switch (command) {
    case '0':
      motorSoftStop();
      Serial.println("Soft stop");
      //digitalWrite(led,LOW);
      break;

    case '1':
      motorHardStop();
      Serial.println("Hard stop");
      break;
    case '2':
      Serial.println("Caso 2");
      motorSetSpeed(SX,((value-48)*28));
      motorSetSpeed(DX,((value-48)*28));
      Serial.println(((value-48)*28));
      break;
    case '3':
      //digitalWrite(led,HIGH);
      motorSetSpeed(SX,value);
      motorSetSpeed(DX,value);
      motorSetDir(SX,CW);
      motorSetDir(DX,CW);

      Serial.println(buffer[0]);

      break;

    default:
      //Serial.println("invalid command");
      break;
  }

}
