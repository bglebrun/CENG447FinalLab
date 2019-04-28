#include "ai.h"

/* stdout stream */
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void getDistances()
{
    for (unsigned char i = 0; i < SERVO_RANGE; i++)
    {
        moveServo(DEG_PER_UNIT * i);
        distances[i] = readUltrasonic();
        _delay_ms(150);
    }
    moveServo(0);
    _delay_ms(100);

    for (unsigned char i = 0; i < SERVO_RANGE; i++)
    {
        fprintf(&mystdout, "distances[%d]: %d\r\n", i, distances[i]);
    }
}

void runAi() { getDistances(); }

void forwardDrive(int speed, int bias)
{
    // If bias is negative->left, positive->right
    switch ((bool)bias > 0)
    {
    case (true):
        // Right bias
        driveForwardBias(speed - bias * BIAS_MULT, speed);
        break;
    case (false):
        // Left bias
        driveForwardBias(speed, speed - bias * BIAS_MULT);
        break;
    }
}

void turnAround(int speed) { turnLeftTimed(speed, TURN_TIMER_LEFT * 2); }

void turnRight(int speed) { turnRightTimed(speed, TURN_TIMER_RIGHT); }

void turnLeft(int speed) { turnLeftTimed(speed, TURN_TIMER_LEFT); }
