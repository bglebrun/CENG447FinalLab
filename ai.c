#include "ai.h"

// void getDistances()
// {
//     // for (unsigned char i = 0; i < SERVO_RANGE; i++)
//     // {
//     //     moveServo(DEG_PER_UNIT * i);
//     //     distances[i] = readUltrasonic();
//     //     _delay_ms(150);
//     // }

//     moveServo(FORWARD_DIST * DEG_PER_UNIT);
//     distances[FORWARD_DIST] = readUltrasonic();
//     _delay_ms(ULTRASONIC_DELAY);

//     // moveServo(LEFT_DIST * DEG_PER_UNIT);
//     // distances[LEFT_DIST] = readUltrasonic();
//     // _delay_ms(ULTRASONIC_DELAY);

//     // moveServo(RIGHT_DIST * DEG_PER_UNIT);
//     // distances[RIGHT_DIST] = readUltrasonic();
//     // _delay_ms(ULTRASONIC_DELAY);

//     // moveServo(FORWARD_DIST * DEG_PER_UNIT);
//     // _delay_ms(ULTRASONIC_DELAY);

//     fprintf(&mystdout, "distances[%d]: %d\r\n", RIGHT_DIST,
//             distances[RIGHT_DIST]);
//     fprintf(&mystdout, "distances[%d]: %d\r\n", FORWARD_DIST,
//             distances[FORWARD_DIST]);
//     fprintf(&mystdout, "distances[%d]: %d\r\n", LEFT_DIST,
//             distances[LEFT_DIST]);
//     // for (unsigned char i = 0; i < SERVO_RANGE; i++)
//     // {
//     //     fprintf(&mystdout, "distances[%d]: %d\r\n", i, distances[i]);
//     // }
// }

// void takeActions()
// {
//     if (distances[RIGHT_DIST] <= CLEAR_DIST)
//     {
//         fprintf(&mystdout, "right is close\r\n");
//         if (distances[FORWARD_DIST] >= CLOSE_DIST)
//         {
//             fprintf(&mystdout, "forward is far and farther than left\r\n");
//             // apply a correction factor to try to keep us near the wall
//             // if (distances[RIGHT_DIST] <= CLOSE_DIST)
//             // {
//             //     correctionFactor = -CLOSE_DIST / distances[RIGHT_DIST];
//             // }
//             // else if (distances[RIGHT_DIST] >= FAR_DIST)
//             // {
//             //     correctionFactor = distances[RIGHT_DIST] / FAR_DIST;
//             // }
//             correctionFactor = 0;
//             // DRIVE STRAIGHT WITH CORRECTION
//             driveForwardBias(BASE_DRIVE_SPEED, correctionFactor);
//             // SET SPEED CORRECTLY FOR ALL FUNCTIONS
//         }
//         else if (distances[LEFT_DIST] >= CLOSE_DIST)
//         {
//             fprintf(&mystdout, "forward is near\r\n");
//             // TURN LEFT
//             turnLeft(BASE_DRIVE_SPEED);
//             // DRIVE STRAIGHT
//             // driveForwardBias(BASE_DRIVE_SPEED, correctionFactor);
//         }
//         else
//         {
//             fprintf(&mystdout, "all directions are near\r\n");
//             // TURN AROUND
//             turnAround(BASE_DRIVE_SPEED);
//         }
//     }
//     else if (distances[FORWARD_DIST] <= CLEAR_DIST ||
//              distances[LEFT_DIST] <= CLEAR_DIST)
//     {
//         fprintf(&mystdout, "right is far and another direction is near\r\n");
//         // TURN RIGHT
//         turnRight(BASE_DRIVE_SPEED);
//         // DRIVE STRAIGHT
//         // driveForwardBias(BASE_DRIVE_SPEED, correctionFactor);
//     }
//     // if all distances are > CLEAR_DIST, halt as we should have
//     // escaped the maze.
// }

void runAi()
{
    // getDistances();
    // takeActions();
    // _delay_ms(ULTRASONIC_DELAY);
    // _delay_ms(2000);
    moveServo(FORWARD_DIST * DEG_PER_UNIT);
    _delay_ms(BASE_ULTRASONIC_DELAY);
    distances[FORWARD_DIST] = readUltrasonic();
    // _delay_ms(BASE_DELAY);

    if (distances[FORWARD_DIST] >= CLOSE_DIST)
    {
        driveForwardBias(BASE_DRIVE_SPEED, BASE_DRIVE_SPEED);
        _delay_ms(BASE_MOVE_DELAY);
    }
    else
    {
        moveServo(RIGHT_DIST * DEG_PER_UNIT);
        _delay_ms(BASE_ULTRASONIC_DELAY);
        distances[RIGHT_DIST] = readUltrasonic();
        // _delay_ms(BASE_DELAY);

        if (distances[RIGHT_DIST] >= CLOSE_DIST)
        {
            turnRight(BASE_TURN_SPEED);
        }
        else
        {
            turnLeft(BASE_TURN_SPEED);
        }
    }

    stop();
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
