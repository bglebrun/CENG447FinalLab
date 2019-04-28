#ifndef _AI_H_
#define _AI_H_
#include "motor_driver.h"
#include "robotIo.h"
#include "servo.h"
#include "ultrasonic.h"
#include "util/delay.h"
#include <stdbool.h>

#define BIAS_MULT 1
#define TURN_TIMER_LEFT 500
#define TURN_TIMER_RIGHT 500

unsigned int distances[SERVO_RANGE];

void forwardDrive(int speed, int bias);
void turnAround(int speed);
void turnRight(int speed);
void turnLeft(int speed);
void getDistances();
void runAi();

#endif
