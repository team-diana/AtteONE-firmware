#include "MotorControl.h"
#include <Arduino.h>

char buffer[4]={0};
char command,value; 

void setup()
{
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  motorInit();
}
void loop() 
{
  if (Serial.available()>0)
  {
    Serial.readBytes(buffer,2);
    command = buffer[0];
    value = buffer [1];
  }

  switch (command)
  {
    case 0:  
      motorSoftStop();
      Serial.println("Soft stop");
      break;

    case 1:
      motorHardStop();
      Serial.println("Hard stop");
      break;

    case 2: 
      motorSetSpeed(0,100);
      motorSetSpeed(1,100);
      motorSetDir(0,0); 
      motorSetDir(1,0);
      break;

    default:
      Serial.println("invalid command");
      break;
  }

}
