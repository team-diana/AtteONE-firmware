#include "MotorControl.h"
#include <Arduino.h>
#include <math.h>

#define M_PI 3.141592653589

motor_t left_Motor;
motor_t right_Motor;

void
all_motorInit () {

    right_Motor.side         = DX;

    right_Motor.motorPin     = 27;
    right_Motor.motorPin2    = 26;
    right_Motor.enablePin    = 15;
    pinMode(right_Motor.motorPin,OUTPUT);
    pinMode(right_Motor.motorPin2,OUTPUT);
    pinMode(right_Motor.enablePin,OUTPUT);


    left_Motor.side         = SX;

    left_Motor.motorPin     = 19;
    left_Motor.motorPin2    = 23;
    left_Motor.enablePin    = 5;

    pinMode(left_Motor.motorPin,OUTPUT);
    pinMode(left_Motor.motorPin2,OUTPUT);
    pinMode(left_Motor.enablePin,OUTPUT);
}

void
pwm_Init() {
    pwmProperties             tmp_Pwm;
    tmp_Pwm.freq            = 3*10e4;
    tmp_Pwm.pwmChannel1     = 0;
    tmp_Pwm.pwmChannel2     = 1;
    tmp_Pwm.resolution      = 8;
    ledcSetup(tmp_Pwm.pwmChannel1, tmp_Pwm.freq, tmp_Pwm.resolution);
    ledcSetup(tmp_Pwm.pwmChannel2, tmp_Pwm.freq, tmp_Pwm.resolution);
    ledcAttachPin(left_Motor.enablePin,tmp_Pwm.pwmChannel1);
    ledcAttachPin(right_Motor.enablePin,tmp_Pwm.pwmChannel2);

}


void drive_Motor(int segno1,int segno2,int val1,int val2) {
  if (segno1==-1 && segno2 ==1) { //SX
        motorSetSpeed(SX,abs(val1));
        motorSetSpeed(DX,abs(val2));
        motorSetDir(SX,CW);
        motorSetDir(DX,CCW);
  }
 else if (segno1==-1 && segno2 ==-1) { //IND
        motorSetSpeed(SX,abs(val1));
        motorSetSpeed(DX,abs(val2));
        motorSetDir(SX,CCW);
        motorSetDir(DX,CCW);
  }
  else if (segno1==1 && segno2 ==-1) { //DX
        motorSetSpeed(SX,abs(val1));
        motorSetSpeed(DX,abs(val2));
        motorSetDir(SX,CCW);
        motorSetDir(DX,CW);
  }
  else if (segno1==1 && segno2 ==1) { //AV
        motorSetSpeed(SX,abs(val1));
        motorSetSpeed(DX,abs(val2));
        motorSetDir(SX,CW);
        motorSetDir(DX,CW);
  }
}

side_t motorSetSpeed(side_t side, int speed) {  //speed is duty cicle

    if (speed >=0) {
        switch(side) {
            case SX:
                digitalWrite(left_Motor.motorPin, HIGH);
                digitalWrite(left_Motor.motorPin2, LOW);
                ledcWrite(left_Motor.side, speed);
                return SX;
            case DX:
                digitalWrite(right_Motor.motorPin, HIGH);
                digitalWrite(right_Motor.motorPin2, LOW);
                ledcWrite(right_Motor.side, speed);
                return DX;
            default:
                Serial.println("invalid side value, 1 or 0");
                return NONE;
        }
    }
}



side_t motorSetDir(side_t side, rot_t rotation) {
    //dir=0 forward, dir=1 backward

    switch(side) {
        case SX:
            // left side
            if (rotation == CCW) {
                //backward rotation
                digitalWrite(left_Motor.motorPin, HIGH);
                digitalWrite(left_Motor.motorPin2, LOW);
                return side;
            }
            else {
                //forward rotation
                digitalWrite(left_Motor.motorPin, LOW);
                digitalWrite(left_Motor.motorPin2, HIGH);
                return side;
            }
        case DX:
            // right side
            if (rotation == CCW) {
                //backward rotation
                digitalWrite(right_Motor.motorPin, HIGH);
                digitalWrite(right_Motor.motorPin2, LOW);
                return side;
            }
            else {
                //forward rotation
                digitalWrite(right_Motor.motorPin, LOW);
                digitalWrite(right_Motor.motorPin2, HIGH);
                return side;
            }
        default:
            Serial.println("invalid side value, 1 or 0");
            return NONE;
    }
}

void
motorSoftStop(void) {

    digitalWrite(left_Motor.motorPin, LOW);
    digitalWrite(left_Motor.motorPin2, LOW);

    digitalWrite(right_Motor.motorPin, LOW);
    digitalWrite(right_Motor.motorPin2, LOW);
}

void
motorHardStop(void) {

    digitalWrite(left_Motor.motorPin, HIGH);
    digitalWrite(left_Motor.motorPin2, HIGH);

    digitalWrite(right_Motor.motorPin, HIGH);
    digitalWrite(right_Motor.motorPin2, HIGH);
}


float polarVectorToSlipSteering (float direction, bool flag_left) {
    double ans=0;
    if (flag_left) direction = -direction;  // For left side --> f(-x)
    double x = (double) ((2*M_PI)/360.0) * direction;

    // Geogebra -> f(x)=If(-π≤x< (-3)/(4)π,-cos(2x),(-3)/(4)π≤x<0,cos((2x)/(3)),0≤x<(π)/(4),cos(2x),(π)/(4)≤x≤π,cos((2)/(3) (x+(π)/(2))))
    if (direction >= -180 && direction < -135 ) {

        ans = (double) -1.0 * cos( 2.0*x );

    } else if (direction >= -135 && direction < 0) {

        ans = (double) cos( 0.666666*x );

    } else if (direction >= 0 && direction < 45) {

        ans = (double) cos(2.0 * x);

    } else if (direction >= 45 && direction <= 180) {

        ans = (double) cos( (0.666666) * (x + (M_PI/2)) );

    } else ans = -0.0000001234;

    return (float) ans;
}

void slipSteeringSaturationProfile (float norm_speed, float direction, float* speed_left, float* speed_right) {
    *speed_left  = norm_speed * polarVectorToSlipSteering(direction, 1);
    *speed_right = norm_speed * polarVectorToSlipSteering(direction, 0);
}
