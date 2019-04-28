#include "ai.h"


void forwardDrive(int speed, int bias) {
    // If bias is negative->left, positive->right
    switch((bool)bias > 0) {
        case(true):
        // Right bias
        driveForwardBias(speed - bias*BIAS_MULT, speed);
        break;
        case(false):
        // Left bias
        driveForwardBias(speed, speed - bias*BIAS_MULT);
        break;
    }

}

void turnRight(int speed) {
    turnRightTimed(speed, TURN_TIMER_RIGHT);
}

void turnLeft(int speed) {
    turnLeftTimed(speed, TURN_TIMER_LEFT);
}