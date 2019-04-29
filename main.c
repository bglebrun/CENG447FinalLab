/*
 * microctrl_base.c
 *
 * Created: 4/12/2019 2:02:14 PM
 * Author : Walter Vaughan
 */

/*** IMPORTANT -- set this name so you can search for it (short, alphanumeric)
 * ***/
#define MY_BT_NAME "BENSQG"

#include "ai.h"
#include "motor_driver.h"
#include "pcint.h"
#include "robotIo.h"
#include "servo.h"
#include "ultrasonic.h"
#include "line_followers.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

void uart_tx(char);
void uart_tx_str(char*);
char uart_rx();
void set_robot_speeds();
void init();

/* stdout stream */
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

volatile unsigned char global_state = 0;
volatile bool left_forward = true;
volatile bool right_forward = true;
volatile bool manual_enable = false;
volatile unsigned char left_speed = 0;
volatile unsigned char right_speed = 0;

void init()
{
    // initialize motor control
    initMotor();

    // initialize servo control
    initServo();

    // initialize ultrasonic
    initUltrasonic();

    // initialize pin change interrupts
    initPCINT();

    // init UART comms
    initUART();

    // Init line followers
    init_line_followers();

    // enable interrupts
    sei();

    left_forward = 0;
    right_forward = 0;
    left_speed = 0;
    right_speed = 0;

}

int main(void)
{
    init();

    /* Set the BT name */
    uart_tx_str("AT+NAME=");
    uart_tx_str(MY_BT_NAME);
    uart_tx(0);
    uart_tx_str("\r\n");
    uart_tx(0);

    /* control output LED on pin 13 -- PORTB5 */
    DDRB = (1 << PORTB5);

    while (overflowCount < 1500)
    {
        // automated mode here
        runAi();
        // fprintf(&mystdout, "overflow count after: %d\r\n", overflowCount);
    }
    manual_enable = true;
    while (1)
    {
        //fprintf(&mystdout, "line followers say: l: %d, m: %d, r: %d\n", line_left(), line_mid(), line_right());
    }

    return -1;
}

/* write a byte to the UART */
void uart_tx(char c)
{
    /* busy-wait until UDRE0 in UCSR0A goes high */
    while ((UCSR0A & (1 << UDRE0)) == 0)
    {
    };
    /* then write the byte */
    UDR0 = c;
}

/* write a string to the UART, not including the null terminator */
void uart_tx_str(char* s)
{
    while (*s)
        uart_tx(*(s++));
}


ISR(USART_RX_vect)
{
    unsigned char inByte = UDR0;
    PORTB |= (1 << PORTB5);

    switch (global_state)
    {
    case 0:
        // look for the start of packet message
        if (inByte == '\n')
        {
            global_state = 1;
        }
        break;
    case 1:
        left_forward = (bool)inByte;
        // fprintf(&mystdout, "right forward: %d\r\n", right_forward);
        global_state = 2;
        break;
    case 2:
        left_speed = inByte;
        // fprintf(&mystdout, "right speed: %d\r\n", right_speed);
        global_state = 3;
        break;
    case 3:
        right_forward = (bool)inByte;
        // fprintf(&mystdout, "left forward: %d\r\n", left_forward);
        global_state = 4;
        break;
    case 4:
        right_speed = inByte;
        // fprintf(&mystdout, "left speed: %d\r\n", left_speed);
        set_robot_speeds();
        global_state = 0;
        break;
    }
    PORTB &= ~(1 << PORTB5);
}

void set_robot_speeds()
{
    if (manual_enable)
    {
        wheelDirection dir_left = left_forward ? FORWARD : BACK;
        wheelDirection dir_right = right_forward ? FORWARD : BACK;
        leftWheels(left_speed, dir_left);
        rightWheels(right_speed, dir_right);
    }
}
