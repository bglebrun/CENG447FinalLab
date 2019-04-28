/*
 * main.c
 *
 * Created: 4/12/2019 2:02:14 PM
 * Author : Ben and Ben
 */

/*** IMPORTANT -- set this name so you can search for it (short, alphanumeric)
 * ***/
#define MY_BT_NAME "BENSQG"

#include "motor_driver.h"
#include "servo.h"
#include "uart.h"
#include "ultrasonic.h"
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

void set_robot_speeds();
void init();
bool extractData(volatile char[]);


volatile bool left_forward = true;
volatile bool right_forward = true;
volatile bool manual_enable = false;
volatile unsigned char left_speed = 0;
volatile unsigned char right_speed = 0;
volatile IOBUFF iobuffer;

void init()
{
    // initialize motor control
    initMotor();

    // init UART comms
    init_uart();

    // enable interrupts
    sei();

    init_uart_buff(iobuffer);
}

int main(void)
{
    init();

    /* Set the BT name */
    uart_tx_str("AT+NAME=");
    uart_tx_str(MY_BT_NAME);
    uart_tx(0);

    /* control output LED on pin 13 -- PORTB5 */
    DDRB = (1 << PORTB5);

    //while (overflowCount < 1500)
    //{
        // automated mode here
    //}
    manual_enable = true;
    while (1)
    {
    }

    return -1;
}

void set_robot_speeds()
{
    wheelDirection dir_a = left_forward ? FORWARD : BACK;
    wheelDirection dir_b = right_forward ? FORWARD : BACK;
    setA(left_speed, dir_a);
    setB(right_speed, dir_b);
}

bool extractData(volatile char iostring[]) {
    left_forward = (bool)iostring[1];
    right_forward = (bool)iostring[2];
    left_speed = iostring[3];
    right_speed = iostring[4];
    return checksum(iostring) == iostring[5];
}

ISR(USART_RX_vect)
{
    PORTB |= (1 << PORTB5);

    //if (manual_enable) {
        char in = UDR0;
        if(in!=0) {
            iobuffer.buffer[iobuffer.index++] = in;
        } else if(in == '\n') {
            //if (extractData(iobuffer.buffer))
                extractData(iobuffer.buffer);
                set_robot_speeds();
            init_uart_buff(iobuffer);
        } else {
            init_uart_buff(iobuffer);
        }
    //}

    PORTB &= ~(1 << PORTB5);
}