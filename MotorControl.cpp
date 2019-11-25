#include "MotorControl.h"
#include <Arduino.h>

void motorInit(void)
{
    pinMode(motor1Pin1, OUTPUT);   // left motor (0)  pin setup
    pinMode(motor1Pin2, OUTPUT);
    pinMode(enable1Pin, OUTPUT);

    pinMode(motor2Pin1, OUTPUT);   // right motor (1) pin setup
    pinMode(motor2Pin2, OUTPUT);
    pinMode(enable2Pin, OUTPUT);
  
    ledcSetup(pwmChannel1, freq, resolution);   // motor A pwm setup
    ledcAttachPin(enable1Pin, pwmChannel1);
    ledcWrite(pwmChannel1, 0);

    ledcSetup(pwmChannel2, freq, resolution);   // motor B pwm setup
    ledcAttachPin(enable2Pin, pwmChannel2);
    ledcWrite(pwmChannel2, 0);
    
}

bool motorSetSpeed(bool side, char speed)
{
    switch(side)
    {   
    case 0:
        // left side
        ledcWrite(pwmChannel1, speed);
        return 1;
        break;
    case 1:
        // right side
        ledcWrite(pwmChannel2, speed);
        return 1;
        break;
    default:
        Serial.println("invalid side value, 1 or 0");
        return 0;
        break;
    }
}

bool motorSetDir(bool side, bool dir) //dir=0 forward, dir=1 backward 
{
    Serial.begin(115200);
    switch(side)
    {
    case 0:
        // left side
        if (dir==1)
        {
            //backward rotation
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, HIGH);
            return 1;
            break;
        }
        else 
        {
            //forward rotation
            digitalWrite(motor1Pin1, HIGH);
            digitalWrite(motor1Pin2, LOW);
            return 1;
            break;
        }
        return 0;
        break;
    case 1:
        // right side
        if (dir==0) 
        {
            //backward rotation
            digitalWrite(motor2Pin1, LOW);
            digitalWrite(motor2Pin2, HIGH);
            return 1;
            break;
        }
        else 
        {
            //forward rotation
            digitalWrite(motor2Pin1, HIGH);
            digitalWrite(motor2Pin2, LOW);
            return 1;
            break;
        }
        return 0;
        break;
    default:
        Serial.println("invalid side value, 1 or 0");
        return 0;
        break;
    }
}

void motorSoftStop(void)
{
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);  
    
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);    
}

void motorHardStop(void)
{
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, HIGH);  
    
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, HIGH);    
}
