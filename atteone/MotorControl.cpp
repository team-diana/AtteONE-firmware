#include "MotorControl.hpp"
#include <Arduino.h>
#include <math.h>

motor_t left_Motor;
motor_t right_Motor;

void motorInitAll () {

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
    left_Motor.enablePin    = 17;

    pinMode(left_Motor.motorPin,OUTPUT);
    pinMode(left_Motor.motorPin2,OUTPUT);
    pinMode(left_Motor.enablePin,OUTPUT);
}

void pwmInit() {
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

void driveMotor(float speed_left, float speed_right) {

    Serial.printf("drive L/R:\t%f %f\n", speed_left, speed_right);

    motorSetSpeed(SX, speed_left);
    motorSetSpeed(DX, speed_right);
}

side_t motorSetSpeed(side_t side, float speed) {  //speed is duty cicle
    float raw_speed = speed;
    int motor_speed  = 0;

    motorSetDir(side, (raw_speed > 0 ? CW : CCW));

    if (raw_speed < -1) raw_speed = -1;
    else if (raw_speed >= -0.002 && raw_speed <= 0.002) raw_speed = 0;
    else if (raw_speed > 1) raw_speed = 1;

    if (raw_speed ==  0) motor_speed = 0;
    else motor_speed = (int) (abs(raw_speed) * 50.0) + 50.0;

    Serial.printf("Speed/Motor Speed:\t%s %f %d\n", (side == SX ? "sx" : "dx"), raw_speed, motor_speed);

    if (raw_speed == 0) {
        if (side == SX) motorLeftSoftStop();
        else if (side == DX) motorRightSoftStop();
    } else {
        switch(side) {
            case SX:
            ledcWrite(left_Motor.side, abs(motor_speed));
            break;
            case DX:
            ledcWrite(right_Motor.side, abs(motor_speed));
            break;
        }
    }
}

side_t motorSetDir(side_t side, rot_t rotation) {

    if (rotation == CW) Serial.printf("Side / Rotation:\t%s %s\n", (side == SX ? "sx" : "dx"), "cw");
    else if (rotation == CCW) Serial.printf("Side / Rotation:\t%s %s\n", (side == SX ? "sx" : "dx"), "ccw");

    if(side == SX) {
        // left side
        if (rotation == CCW) {
            //backward rotation
            digitalWrite(left_Motor.motorPin, HIGH);
            digitalWrite(left_Motor.motorPin2, LOW);
        }
        else {
            //forward rotation
            digitalWrite(left_Motor.motorPin, LOW);
            digitalWrite(left_Motor.motorPin2, HIGH);
        }
    } else if (side == DX) {
        // right side
        if (rotation == CCW) {
            //backward rotation
            digitalWrite(right_Motor.motorPin, HIGH);
            digitalWrite(right_Motor.motorPin2, LOW);
        } else {
            //forward rotation
            digitalWrite(right_Motor.motorPin, LOW);
            digitalWrite(right_Motor.motorPin2, HIGH);
        }
    } else {
        Serial.println("invalid side value, 1 or 0");
        return NONE;
    }
}

void motorLeftSoftStop(void) {

    ledcWrite(left_Motor.side, 0);
    digitalWrite(left_Motor.motorPin, LOW);
    digitalWrite(left_Motor.motorPin2, LOW);

}

void motorRightSoftStop(void) {
    ledcWrite(right_Motor.side, 0);

    digitalWrite(right_Motor.motorPin, LOW);
    digitalWrite(right_Motor.motorPin2, LOW);
}

void motorSoftStop(void) {

    motorLeftSoftStop();
    motorRightSoftStop();
}

void motorHardStop(void) {

    ledcWrite(left_Motor.side, 0);
    ledcWrite(right_Motor.side, 0);

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
    if      (direction >= -180 && direction < -135) ans = (double) -1.0 * cos(2.0 * x);
    else if (direction >= -135 && direction <    0) ans = (double) cos(0.666666 * x);
    else if (direction >=    0 && direction <   45) ans = (double) cos(2.0 * x);
    else if (direction >=   45 && direction <= 180) ans = (double) cos(0.666666 * (x + (M_PI/2)) );
    else ans = -0.0000001234;

    return (float) ans;
}

void slipSteeringSaturationProfile (float norm_speed, float direction, float* speed_left, float* speed_right) {
    *speed_left  = norm_speed * polarVectorToSlipSteering(direction, 1);
    *speed_right = norm_speed * polarVectorToSlipSteering(direction, 0);
}
