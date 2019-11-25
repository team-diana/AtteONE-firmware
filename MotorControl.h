#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H
#include <arduino.h>

// left Motors (0)
const int motor1Pin1 = 27; 
const int motor1Pin2 = 26; 
const int enable1Pin = 14; 

// right Motors (1)
const int motor2Pin1 = 17;
const int motor2Pin2 = 16;
const int enable2Pin = 4;

// Setting PWM properties
const int freq = 30000;
const int pwmChannel1 = 0;
const int pwmChannel2 = 1;
const int resolution = 8;

void motorInit(void);
bool motorSetSpeed(bool side, char speed);
bool motorSetDir(bool side, bool dir);
void motorHardStop(void);
void motorSoftStop(void);

#endif
