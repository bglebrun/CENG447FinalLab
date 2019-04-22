#include "servo.h"

/* stdout stream */
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void initServo()
{
    // set pin and timers
    // set to 1 as output
    setBit(DDRD, 3);

    // init TC2

    initTimer2();
}

void moveServo(unsigned char deg) { OCR2B = mapAngle(deg); }

// mapping assumes 0 degrees is full CW (right) and 180 degrees is full CCW
// (left)
unsigned char mapAngle(unsigned char angleDeg)
{
    unsigned char result;
    if (angleDeg <= 0)
    {
        result = DEG_MAP_MIN;
    }
    else if (angleDeg >= 180)
    {
        result = DEG_MAP_MAX;
    }
    else
    {
        result = DEG_MAP_MIN + (angleDeg / DEG_PER_UNIT);
    }

    fprintf(&mystdout, "result map: %d\r\n", result);

    return result;
}
