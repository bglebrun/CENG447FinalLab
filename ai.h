#ifndef _AI_H_
#define _AI_H_
#include "motor_driver.h"
#include "robotIo.h"
#include "servo.h"
#include "ultrasonic.h"
#include "util/delay.h"
#include <stdbool.h>

#define BIAS_MULT 0.2
#define TURN_TIMER_LEFT 500
#define TURN_TIMER_RIGHT 500

// distance defines
#define RIGHT_DIST 0
#define LEFT_DIST (SERVO_RANGE - 1)
#define FORWARD_DIST ((unsigned char)(SERVO_RANGE / 2))
#define CLEAR_DIST 50
#define CLOSE_DIST 20

// control defines
#define BASE_DRIVE_SPEED 192
#define BASE_TURN_SPEED 250
#define BASE_MOVE_DELAY 250
#define BASE_ULTRASONIC_DELAY 1000

unsigned int distances[SERVO_RANGE];
unsigned char correctionFactor;

void forwardDrive(int speed, int bias);
void turnAround(int speed);
void turnRight(int speed);
void turnLeft(int speed);
void getDistances();
void runAi();
void takeActions();

#endif
