#include "robotIo.h"

void initUART()
{
    // init uart
    // UCSR0B |= 0x98;
    // UCSR0C |= 0x06;
    UCSR0B |= UCSR0B_CONFIG;
    UCSR0C |= UCSR0C_CONFIG;

    UBRR0L = BAUD_PRESCALE;
    UBRR0H = (BAUD_PRESCALE >> 8);
}

int uart_putchar(char c, FILE* stream)
{
    if (c == '\n')
        uart_putchar('\r', stream);
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}
