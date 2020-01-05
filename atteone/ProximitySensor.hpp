#ifndef __PROXTIMITYSENSOR_HPP__
#define __PROXTIMITYSENSOR_HPP__
#include <Arduino.h>

#define PIN_TRIGGER   18
#define PIN_ECHO      5
/*#define PIN_SERVO     21

typedef enum{ SX,
              DX,
              NONE} side_t;

typedef enum{ CW,
              CCW} rot_t;

int right_distance=0, left_distance=0, middle_distance=0;*/


unsigned long duration;
int distance;

float sensor(void);
void setupsensor(void);


#endif //__PROXTIMITYSENSOR_HPP__
