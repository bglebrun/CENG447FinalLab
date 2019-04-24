/*
 * microctrl_base.c
 *
 * Created: 4/12/2019 2:02:14 PM
 * Author : Walter Vaughan
 */

/*** IMPORTANT -- set this name so you can search for it (short, alphanumeric)
 * ***/
#define MY_BT_NAME "BENSQ"

#define F_CPU (16000000)
// #define BAUDRATE (9600)
// #define BAUD_PRESCALE (F_CPU / BAUDRATE / 16 - 1)

#include "motor_driver.h"
#include "robotIo.h"
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
volatile unsigned char left_speed = 0;
volatile unsigned char right_speed = 0;

void init()
{
    // initialize motor control
    initMotor();

    // init UART comms
    initUART();

    // enable interrupts
    sei();

    fprintf(&mystdout, "init done\r\n");
}

int main(void)
{
    // char r; /* received character on UART */

    // /* startup UART @ 9600 8n1 asynchronous, receive & transmit enabled */
    // UBRR0H = (unsigned char)(BAUD_PRESCALE >> 8);
    // UBRR0L = (unsigned char)BAUD_PRESCALE;
    // UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    // UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    init();

    /* Set the BT name */
    // uart_tx_str("AT+NAME=");
    // uart_tx_str(MY_BT_NAME);
    // uart_tx(0);
    fprintf(&mystdout, "AT+NAME=%s\r\n", MY_BT_NAME);

    /* control output LED on pin 13 -- PORTB5 */
    DDRB = (1 << PORTB5);

    while (1)
    {
        // r = uart_rx();
        // if (r == 'h')
        //     PORTB |= (1 << PORTB5);
        // if (r == 'l')
        //     PORTB &= ~(1 << PORTB5);
    }

    return -1;
}

/* write a byte to the UART */
// void uart_tx(char c)
// {
//     /* busy-wait until UDRE0 in UCSR0A goes high */
//     while ((UCSR0A & (1 << UDRE0)) == 0)
//         ;
//     /* then write the byte */
//     UDR0 = c;
// }

/* write a string to the UART, not including the null terminator */
// void uart_tx_str(char* s)
// {
//     while (*s)
//         uart_tx(*(s++));
// }

/* return whatever byte is in the RX, if there is one. Return 0 otherwise */
// char uart_rx()
// {
//     if (UCSR0A & (1 << RXC0))
//         return UDR0;
//     else
//         return 0;
// }

ISR(USART_RX_vect)
{
    char inByte = UDR0;
    PORTB |= (1 << PORTB5);

    switch (global_state)
    {
    case 0:
        left_forward = (bool)inByte;
        fprintf(&mystdout, "left forward: %d\r\n", left_forward);
        global_state = 1;
        break;
    case 1:
        left_speed = inByte;
        fprintf(&mystdout, "left speed: %d\r\n", left_speed);
        global_state = 2;
        break;
    case 2:
        right_forward = (bool)inByte;
        fprintf(&mystdout, "right forward: %d\r\n", right_forward);
        global_state = 3;
        break;
    case 3:
        right_speed = inByte;
        fprintf(&mystdout, "right speed: %d\r\n", right_speed);
        set_robot_speeds();
        global_state = 0;
        break;
    }
    PORTB &= ~(1 << PORTB5);
}

void set_robot_speeds()
{
    wheelDirection dir_a = left_forward ? FORWARD : BACK;
    wheelDirection dir_b = right_forward ? FORWARD : BACK;
    setA(left_speed, dir_a);
    setB(right_speed, dir_b);
}