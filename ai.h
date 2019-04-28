#ifndef _AI_H_
#define _AI_H_
#include "motor_driver.h"
#include <stdbool.h>
#define BIAS_MULT 1
#define TURN_TIMER_LEFT 50
#define TURN_TIMER_RIGHT 50

void forwardDrive(int speed, int bias);
void turnRight(int speed);
void turnLeft(int speed);

#endif
