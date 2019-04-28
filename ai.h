#ifndef _AI_H_
#define _AI_H_
#include "motor_driver.h"
#include <stdbool.h>
#define BIAS_MULT 1

void forwardDrive(int speed, int bias);
void turnRight(int speed);
void turnLeft(int speed);

#endif
