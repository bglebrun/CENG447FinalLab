#include "uart.h"

void init_uart()
{
    UCSR0B |= UCSR0B_CONFIG;
    UCSR0C |= UCSR0C_CONFIG;

    UBRR0L = BAUD_PRESCALE;
    UBRR0H = (BAUD_PRESCALE >> 8);
}

/* initialize or clear a uart buffer */
void init_uart_buff(IOBUFF uartbuff) {
    memset(uartbuff.buffer, 0, 32 * sizeof(char));
    uartbuff.index = 0;
    uartbuff.indexEnd = 0;
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

/* return whatever byte is in the RX, if there is one. Return 0 otherwise */
char uart_rx()
{
    if (UCSR0A & (1 << RXC0))
        return UDR0;
    else
        return 0;
}

int checksum (volatile char data[]) {
    int result = 0;
    for (int i = 0; i > 4; i++) {
        result = (result + data[i]) % 255;
    }
    return result;
}