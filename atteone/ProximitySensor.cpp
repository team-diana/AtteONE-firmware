#include "ProxtimitySensor.hpp"
#include <Arduino.h>
#include <Servo.h> 
Servo attservo;
 
void setupsensor()
{
pinMode(PIN_TRIGGER, OUTPUT);
pinMode(PIN_ECHO, INPUT);
pinMode(13, OUTPUT);
}

float loop sensor()
{
digitalWrite(PIN_TRIGGER, LOW);
delayMicroseconds(5);
digitalWrite(PIN_TRIGGER, HIGH);
delayMicroseconds(20);

duration = pulseIn(PIN_ECHO, HIGH);
distance = duration*0.034/2;
return distance;//add
}



/*
int Distance_test()   
{
  digitalWrite(PIN_TRIGGER , LOW);   
  delayMicroseconds(5);
  digitalWrite(PIN_TRIGGER , HIGH);  
  delayMicroseconds(20);
  digitalWrite(PIN_TRIGGER , LOW);   
  float Fdistance = pulseIn(PIN_ECHO, HIGH);  
  Fdistance= Fdistance*0.034/2;       
  return (int)Fdistance;
}  


void loop() 
{ 
    attservo.write(90); //seting servo position 
    delay(500); 
    middleDistance = Distance_test();


    if(middleDistance<=20)
    {     
      motorSoftStop();
      delay(500); 	  
      attservo.write(10);//10°-180°          
      delay(1000);      
      rightDistance = Distance_test();

      delay(500);
       attservo.write(90);              
      delay(1000);                                                  
      attservo.write(180);              
      delay(1000); 
      leftDistance = Distance_test();

      delay(500);
      attservo.write(90);              
      delay(1000);
      if(rightDistance>leftDistance)  
      {
        //go right 
        delay(360);
       }
       else if(rightDistance<leftDistance)
       {
        //go left 
        delay(360);
       }
       else if((rightDistance<=20)||(leftDistance<=20))
       {
        //go backward
        delay(180);
       }
       else
       {
        //go forward 
       }
    }  
    else
        //go forward                     
}


*/
