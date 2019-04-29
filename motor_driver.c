#include "motor_driver.h"

// motors use pins

/*
 * IN 1 & IN 2 & EN A
 *
 * IN 3 & IN 4 & EN B
 */

#define DELAY_COUNT(time, rate) time / rate + (time % rate == 0 ? 0 : 1)

// interrupt counter for motor A
volatile unsigned long MAIC;
unsigned long targetCount;

void initMotor()
{
    // Init port B for output
    setBit(DDRD, H_IN1);
    setBit(DDRB, H_IN2);
    setBit(DDRB, H_IN3);
    setBit(DDRB, H_IN4);
    clearBit(PORTD, H_IN1);
    clearBit(PORTB, H_IN2);
    clearBit(PORTB, H_IN3);
    clearBit(PORTB, H_IN4);

    initTimer0();
}

// Right Wheels
void rightWheels(unsigned char speed, wheelDirection direction)
{
    switch (direction)
    {
    case FORWARD:
        setBit(PORTD, H_IN1);
        clearBit(PORTB, H_IN2);
        break;
    case BACK:
        setBit(PORTB, H_IN2);
        clearBit(PORTD, H_IN1);
        break;
    }
    OCR0A = ((ABSOLUTE_BIAS > 0) ? speed - abs(ABSOLUTE_BIAS) : speed);
}

// Left wheels
void leftWheels(unsigned char speed, wheelDirection direction)
{
    switch (direction)
    {
    case BACK:
        setBit(PORTB, H_IN3);
        clearBit(PORTB, H_IN4);
        break;
    case FORWARD:
        setBit(PORTB, H_IN4);
        clearBit(PORTB, H_IN3);
        break;
    }
    OCR0B = ((ABSOLUTE_BIAS < 0) ? speed - abs(ABSOLUTE_BIAS) : speed);
}

void turnLeftTimed(unsigned char speed, int time_ms)
{
    getNumInterruptsForDuration(time_ms);
    leftWheels(speed, FORWARD);
    rightWheels(speed, BACK);
    delayUntilTargetCount();
}

void turnRightTimed(unsigned char speed, int time_ms)
{
    getNumInterruptsForDuration(time_ms);
    leftWheels(speed, BACK);
    rightWheels(speed, FORWARD);
    delayUntilTargetCount();
}

void driveForwardTimed(unsigned char speed, int time_ms)
{
    getNumInterruptsForDuration(time_ms);
    rightWheels(speed, FORWARD);
    leftWheels(speed, FORWARD);
    delayUntilTargetCount();
}

void driveForward(unsigned char speed)
{
    rightWheels(speed, FORWARD);
    leftWheels(speed, FORWARD);
}

void driveForwardBias(unsigned char leftSpeed, unsigned char rightSpeed) {
    leftWheels(leftSpeed, FORWARD);
    rightWheels(rightSpeed, FORWARD);
}

void driveBackward(unsigned char speed)
{
    rightWheels(speed, BACK);
    leftWheels(speed, BACK);
}

void driveBackwardTimed(unsigned char speed, int time_ms)
{
    getNumInterruptsForDuration(time_ms);
    rightWheels(speed, BACK);
    leftWheels(speed, BACK);
    delayUntilTargetCount();
}

void stop()
{
    rightWheels(0, FORWARD);
    leftWheels(0, FORWARD);
}

void delayUntilTargetCount()
{
    while (MAIC <= targetCount)
    {
    };
    MAIC = 0;
}

void getNumInterruptsForDuration(int duration_ms)
{
    int prescaler_choice = TCCR0B & 0x07;

    switch (prescaler_choice)
    {
    case 0: // no clock...no motor???
        // ERROR: without a clock a) the motors aren't running and
        // b) we can't wait a duration...
        targetCount = 0;
        break;
    case 1: // 1 prescaler, one interrupt ~ every 0.02ms
        targetCount = DELAY_COUNT(100 * duration_ms, 2);
        break;
    case 2: // 8 prescaler, one interrupt ~ every 0.1ms
        targetCount = DELAY_COUNT(10 * duration_ms, 1);
        break;
    case 3: // 64 prescaler, one interrupt ~ every 1ms
        targetCount = DELAY_COUNT(duration_ms, 1);
        break;
    case 4: // 256 prescaler, one interrupt ~ every 4ms
        targetCount = DELAY_COUNT(duration_ms, 4);
        break;
    case 5: // 1024 prescaler, one interrupt ~ every 16ms
        targetCount = DELAY_COUNT(duration_ms, 16);
        break;
    case 6:
    case 7:
        targetCount = 0;
        // ERROR: 6 and 7 are external clocks and we can't predict them
    }
    // reset the counter variable
    MAIC = 0;
}

ISR(TIMER0_COMPA_vect) { MAIC++; }
