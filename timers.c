#include "timers.h"

// Getter for overflow status
bool getOverflowStatus() { return TimerOverflow; }

// Overflow vector
ISR(TIMER1_OVF_vect)
{
    // Timer 1 overflow
    TimerOverflow = true;
}

void initTimer0()
{
    // set both motors to start off
    OCR0A = 0;
    OCR0B = 0;

    // start TCNT0 at 0
    TCNT0 = 0x00;

    // Enable fast pwm mode for DC motor output
    TCCR0A = 0xA3;
    TCCR0B = 0x05; // 1024 prescaler
    // TCCR0B = 0x04; // 256 prescaler
    // TCCR0B = 0x03; // 64 prescaler
    // TCCR0B = 0x02; // 8 prescaler
    // TCCR0B = 0x01; // 1 prescaler

    // Enable counter match interrupt for counter A
    TIMSK0 = 0x02;
}

void initTimer1()
{
    /*
     * Timer 1
     * Pins 7:4 zeroes
     * tccr1a high byte zeroes
     * 16 us
     */
    OCR1A = 0;
    OCR1B = 0;
    // Start tcount at 1
    TCNT1 = 0x00;
    // Normal mode
    TCCR1A = 0x00;

    // enable overflow interrupt
    TIMSK1 |= (1 << TOIE1);

    // ensure we start with the timer off
    turnoffTimer1();
}

void initTimer2()
{
    // phase correct with 1024 prescaler gets us a high enough max time
    // use OCR2A to set TOP (WGM 5) such that we interrupt as close to 2ms
    // as possible

    // OCR2A = 16MHz/(2*1024*50Hz)
    // we choose 50Hz because 50Hz==20ms
    // OCR2A = 156.25, we can't have fractions so we use 156
    OCR2A = 156;
    // OCR2B determines the width of the pulse
    // OCR2B = 16MHz*(pulse width in ms)/(2*1024)
    // 1ms = 8, 1.5ms = 12, 2ms = 16
    // start facing forward
    OCR2B = 12;

    // start TC2 at 0
    TCNT2 = 0x00;

    // phase corrected PWM mode with 1024 clock prescale
    TCCR2A |= (0 << COM2A1) | (0 << COM2A0) | (1 << COM2B1) | (0 << COM2B0) |
              (0 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << WGM22) | (1 << CS22) | (1 << CS21) | (1 << CS20);
}

/******************************************************************************
 *
 * Magical Timer land
 *
 * Prescaler value goes in TCCR1B
 * 0 - off
 * 1 - no prescaling
 * 2 - clock /8
 * 3 - clock /64
 * 4 - clock /256 - 16us
 * 5 - clock /1024 - 64us
 * source -
 * https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
 */

// convenience function to turn the timer off
void turnoffTimer1() { TCCR1B &= 0x00; }

// convenience function to turn the timer back on with a default prescaler
void turnonTimer1() { TCCR1B |= 0x05; }

// Reads from timer 1 counter
unsigned int TIM16_ReadTCNT1()
{
    unsigned char sreg;
    unsigned int i;
    // Save global interrupt flag
    sreg = SREG;
    // Disable interrupts
    cli();
    // Read TCNT1 into i
    i = TCNT1;
    // Restore global interrupt flag
    SREG = sreg;
    return i;
}

// Sets timer 1 counter
void TIM16_WriteTCNT1(unsigned int i)
{
    unsigned char sreg;
    // Save global interrupt flag
    sreg = SREG;
    // Disable interrupts
    cli();
    // Set TCNT1  to i
    TCNT1 = i;
    // Restore global interrupt flag
    SREG = sreg;
}
