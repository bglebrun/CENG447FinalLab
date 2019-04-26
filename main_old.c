#define F_CPU 16000000
#include "bit_macros.h"
#include "pcint.h"
#include "robotIo.h"
#include "servo.h"
#include "ultrasonic.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

void printAngleDistance(unsigned char angle, unsigned int distance);

/* stdout stream */
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void Init()
{
    // configure UART and I/O
    initUART();

    // initialize pin change interrupts
    initPCINT();

    // configure ultrasonic range sensor
    initUltrasonic();

    // configure servo
    initServo();

    // Enable global interrupts
    sei();
}

int main()
{
    Init();
    while (1)
    {
        fprintf(&mystdout, "Starting tests:\r\n");
        // start centered
        moveServo(90);
        printAngleDistance(90, readUltrasonic());
        _delay_ms(500);
        // full CCW (left)
        moveServo(180);
        printAngleDistance(180, readUltrasonic());
        _delay_ms(500);
        // full CW (right)
        moveServo(0);
        printAngleDistance(0, readUltrasonic());
        _delay_ms(500);
        for (unsigned char i = 0; i <= 180; i += 15)
        {
            moveServo(i);
            printAngleDistance(i, readUltrasonic());
            _delay_ms(500);
        }
        _delay_ms(500);
    }
    return 1;
}

void printAngleDistance(unsigned char angle, unsigned int distance)
{
    fprintf(&mystdout, "Position: %d degrees | Distance: %d cm\r\n", angle,
            distance);
}
