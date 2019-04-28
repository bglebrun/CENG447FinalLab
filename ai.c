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
