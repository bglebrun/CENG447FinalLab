#ifndef _AI_H_
#define _AI_H_

#include "robotIo.h"
#include "servo.h"
#include "ultrasonic.h"
#include "util/delay.h"

unsigned int distances[SERVO_RANGE];

void getDistances();
void runAi();

#endif
