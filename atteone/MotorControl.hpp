#ifndef __MOTORCONTROL_HPP__
#define __MOTORCONTROL_HPP__
#include <Arduino.h>

typedef enum{ SX,
              DX,
              NONE} side_t;

typedef enum{ CW,
              CCW} rot_t;

typedef struct _motor_t {

        int     motorPin;
        int     motorPin2;
        int     enablePin;

        side_t  side;

        }motor_t;

typedef struct _pwmProperties {

        int     freq;
        int     pwmChannel1;
        int     pwmChannel2;
        int     resolution;

        }pwmProperties;

// Motor Pinout
/*************************
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
**************************/

side_t motorSetSpeed(side_t side, float speed);
void   motorInitAll();
side_t motorSetDir(side_t side, rot_t dir);
void   motorHardStop(void);
void   motorSoftStop(void);
void   motorLeftSoftStop(void);
void   motorRightSoftStop(void);
void   pwmInit(void);
void   driveMotor(float speed_left, float speed_right);
float  polarVectorToSlipSteering (float direction, bool flag_left);
void   slipSteeringSaturationProfile (float norm_speed, float direction, float* speed_left, float* speed_right);

#endif //__MOTORCONTROL_HPP__
