#include "pcint.h"

/* stdout stream */
// static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL,
// _FDEV_SETUP_WRITE);

void initPCINT()
{
    PCMSK1 |= PCMSK1_CONFIG;
    PCICR |= PCICR_CONFIG;
}

// specific pin change interrupt handler that is less general, but
// should be faster so less time is spent in the interrupt and
// measurements are more accurate.
//
// Expects only PCINT12 active (the ultrasonic sensor input)
ISR(PCINT1_vect)
{
    // used to detect whether pcint is going high->low or low->high
    // a value of false indicates we are low->high, a value of true indicates
    // high->low.
    static bool highEdge = false;

    if (highEdge)
    {
        turnoffTimer1();
        responseAvailable = true;
    }
    else
    {
        TIM16_WriteTCNT1(0);
        turnonTimer1();
        responseAvailable = false;
    }

    highEdge = !highEdge;
}
