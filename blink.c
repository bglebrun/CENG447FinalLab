/*
 * microctrl_base.c
 *
 * Created: 4/12/2019 2:02:14 PM
 * Author : Walter Vaughan
 */

/*** IMPORTANT -- set this name so you can search for it (short, alphanumeric)
 * ***/
#define MY_BT_NAME "changeme"

#define F_CPU (16000000)
#define BAUDRATE (9600)
#define BAUD_PRESCALE (F_CPU / BAUDRATE / 16 - 1)

#include <avr/io.h>
#include <util/delay.h>

void uart_tx(char);
void uart_tx_str(char*);
char uart_rx();

int main(void)
{
    char r; /* received character on UART */

    /* startup UART @ 9600 8n1 asynchronous, receive & transmit enabled */
    UBRR0H = (unsigned char)(BAUD_PRESCALE >> 8);
    UBRR0L = (unsigned char)BAUD_PRESCALE;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);

    /* Set the BT name */
    uart_tx_str("AT+NAME=");
    uart_tx_str(MY_BT_NAME);
    uart_tx(0);

    /* control output LED on pin 13 -- PORTB5 */
    DDRB = (1 << PORTB5);

    while (1)
    {
        r = uart_rx();
        if (r == 'h')
            PORTB |= (1 << PORTB5);
        if (r == 'l')
            PORTB &= ~(1 << PORTB5);
    }
}

/* write a byte to the UART */
void uart_tx(char c)
{
    /* busy-wait until UDRE0 in UCSR0A goes high */
    while ((UCSR0A & (1 << UDRE0)) == 0)
        ;
    /* then write the byte */
    UDR0 = c;
}

/* write a string to the UART, not including the null terminator */
void uart_tx_str(char* s)
{
    while (*s)
        uart_tx(*(s++));
}

/* return whatever byte is in the RX, if there is one. Return 0 otherwise */
char uart_rx()
{
    if (UCSR0A & (1 << RXC0))
        return UDR0;
    else
        return 0;
}
