#include "MotorControl.h"
#include <Arduino.h>

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
                digitalWrite(left_Motor.motorPin, LOW);
                digitalWrite(left_Motor.motorPin2, HIGH);
                return side;
            }
            else {
                //forward rotation
                digitalWrite(left_Motor.motorPin, HIGH);
                digitalWrite(left_Motor.motorPin2, LOW);
                return side;
            }
        case DX:
            // right side
            if (rotation == CW) {
                //backward rotation
                digitalWrite(right_Motor.motorPin, LOW);
                digitalWrite(right_Motor.motorPin2, HIGH);
                return side;
            }
            else {
                //forward rotation
                digitalWrite(right_Motor.motorPin, HIGH);
                digitalWrite(right_Motor.motorPin2, LOW);
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
