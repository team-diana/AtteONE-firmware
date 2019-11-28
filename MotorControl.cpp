#include "MotorControl.h"
//#include <Arduino.h>

void
all_motorInit () {

    right_Motor.side         = DX;

    right_Motor.motorPin     = 27;
    right_Motor.motorPin2    = 26;
    right_Motor.enablePin    = 14;   


    left_Motor.side         = SX;


    left_Motor.motorPin     = 17;
    left_Motor.motorPin2    = 16;
    left_Motor.enablePin    = 4;
}

void
pwm_Init() {
    pwmProperties           tmp_Pwm;
    tmp_Pwm.freq            = 3*10e5;
    tmp_Pwm.pwmChannel1     = 0;
    tmp_Pwm.pwmChannel2     = 1;
    tmp_Pwm.resolution      = 8;
}


side_t motorSetSpeed(side_t side, char speed) {
    
    if (speed >=0) {
        switch(side) {   
            case SX:
                ledcWrite(left_Motor.side, speed);
                return SX;
            case DX:
                ledcWrite(right_Motor.side, speed);
                return DX;
            default:
                Serial.println("invalid side value, 1 or 0");
                return NONE;
        }
    }
}



side_t
motorSetDir(side_t side, rot_t rotation) {
    //dir=0 forward, dir=1 backward 

    switch(side) {
        case SX:
            // left side
            if (CCW) {
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
            if (!CW) {
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
