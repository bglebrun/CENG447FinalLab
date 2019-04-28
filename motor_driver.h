#ifndef _MOTOR_DRIVER_H_
#define _MOTOR_DRIVER_H_
#include "bit_macros.h"
#include "pin_map.h"
#include "timers.h"
#include "util/delay.h"
#include <avr/interrupt.h>
#include <avr/io.h>

#define TURN_DELAY_CIRCLE 250
#define TURN_DELAY_SQUARE 500
#define DRIVE_DELAY 500
// positive->right turn bias, negative->left turn bias
#define ABSOLUTE_BIAS 0
#define abs(x) (x > 0 ? x : (-1) * x)


typedef enum WHEEL_DIRECTION
{
    FORWARD = 0,
    BACK
} wheelDirection;

void initMotor();
void rightWheels(unsigned char speed, wheelDirection direction);
void leftWheels(unsigned char speed, wheelDirection direction);
void stop();
void turnLeftTimed(unsigned char speed, int time_ms);
void turnRightTimed(unsigned char speed, int time_ms);
void driveFoward(unsigned char speed);
void driveForwardBias(unsigned char leftSpeed, unsigned char rightSpeed);
void driveBackward(unsigned char speed);
void driveForwardTimed(unsigned char speed, int time_ms);
void driveBackwardTimed(unsigned char speed, int time_ms);
void delayUntilTargetCount();
void getNumInterruptsForDuration(int duration_ms);

#endif
