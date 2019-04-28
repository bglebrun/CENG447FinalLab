#include "ai.h"

/* stdout stream */
// static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,
// _FDEV_SETUP_WRITE);

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

    // for (unsigned char i = 0; i < SERVO_RANGE; i++)
    // {
    //     fprintf(&mystdout, "distances[%d]: %d\r\n", i, distances[i]);
    // }
}

void takeActions()
{
    if (distances[RIGHT_DIST] <= CLEAR_DIST)
    {
        if (distances[FORWARD_DIST] >= CLEAR_DIST)
        {
            // apply a correction factor to try to keep us near the wall
            if (distances[RIGHT_DIST] <= CLOSE_DIST)
            {
                correctionFactor = -CLOSE_DIST / distances[RIGHT_DIST];
            }
            else if (distances[RIGHT_DIST] >= FAR_DIST)
            {
                correctionFactor = distances[RIGHT_DIST] / FAR_DIST;
            }
            // TODO: DRIVE STRAIGHT WITH CORRECTION
        }
        else if (distances[LEFT_DIST] >= CLEAR_DIST)
        {
            // TODO: TURN LEFT
            // TODO: DRIVE STRAIGHT
        }
        else
        {
            // TODO: TURN AROUND
        }
    }
    else
    {
        // TODO: TURN RIGHT
        // TODO: DRIVE STRAIGHT
    }
}

void runAi()
{
    getDistances();
    takeActions();
}

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
