#ifndef _SERVO_H_
#define _SERVO_H_
#include "bit_macros.h"
#include "pin_map.h"
#include "robotIo.h"
#include "timers.h"
#include <avr/io.h>

#define DEG_MAP_MAX 18
#define DEG_MAP_MIN 4
#define DEG_PER_UNIT (180 / (DEG_MAP_MAX - DEG_MAP_MIN))

void initServo();
void moveServo(unsigned char deg);
unsigned char mapAngle(unsigned char angleDeg);

#endif
