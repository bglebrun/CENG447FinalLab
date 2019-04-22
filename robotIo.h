#ifndef _ROBOT_IO_H_
#define _ROBOT_IO_H_
#include <avr/io.h>
#include <stdio.h>

/* baudrate configuration */
#define USART_BAUDRATE 9600
/* baudrate prescaler configuration */
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

/* configurations for the UART device register B*/
#define UCSR0B_CONFIG                                                          \
    (0 << RXCIE0) | (0 << TXCIE0) | (0 << UDRIE0) | (1 << RXEN0) |             \
        (1 << TXEN0) | (0 << UCSZ02) | (0 << RXB80) | (0 << TXB80);
/* configurations for the UART device register C*/
#define UCSR0C_CONFIG                                                          \
    (0 << UMSEL01) | (0 << UMSEL00) | (0 << UPM01) | (0 << UPM00) |            \
        (0 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00) | (0 << UCPOL0);

/* handles inserting characters into the output stream */
int uart_putchar(char c, FILE* stream);
/* initializes UART communications using the defines above*/
void initUART();

#endif
