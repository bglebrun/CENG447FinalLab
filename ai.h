#ifndef _AI_H_
#define _AI_H_
#include "motor_driver.h"
#include <stdbool.h>
#define BIAS_MULT 1

static int bias = 0;

void forwardDrive(int, int);
void turnRight(int);
void turnLeft(int);

#endif
