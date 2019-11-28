#include "../libs/MotorControl.h"

char buffer[4]={0};
char command,value; 

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  all_motorInit();                  //Initialise motors pins
}
void loop() {

  if (Serial.available()>0) {
    Serial.readBytes(buffer,2);
    command     = buffer[0];
    value       = buffer [2];
  }

  switch (command) {
    case '0':  
      motorSoftStop();
      Serial.println("Soft stop");
      break;

    case '1':
      motorHardStop();
      Serial.println("Hard stop");
      break;

    case '2': 
      motorSetSpeed(SX,value);
      motorSetSpeed(DX,value);
      motorSetDir(SX,CW); 
      motorSetDir(DX,CW);
      break;

    default:
      Serial.println("invalid command");
      break;
  }

}