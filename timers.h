#ifndef _TIMERS_H_
#define _TIMERS_H_
#include "globals.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>

void turnoffTimer1();
void turnonTimer1();
void initTimer0();
void initTimer1();
void initTimer2();
void TIM16_WriteTCNT1(unsigned int i);
unsigned int TIM16_ReadTCNT1();
bool getOverflowStatus();

#endif
