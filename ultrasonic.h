#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_
#include "bit_macros.h"
#include "globals.h"
#include "pcint.h"
#include "pin_map.h"
// #include "robotIo.h"
#include "timers.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

/*
 *
 * Ultrasonic sensor: A4 A5 (portc 4 5)
 * Servo Motor: 3 (portd 3)
 */

void initUltrasonic();
void triggerUltrasonic();
unsigned int readUltrasonic();

#endif
