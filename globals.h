#ifndef _GLOBALS_H_
#define _GLOBALS_H_
#include <stdbool.h>

/******************************************************************************
 *
 * Overflow detection helpers
 *
 * Flag for overflow:
 * false = no overflow.
 * true = overflow happened in test
 */
volatile bool TimerOverflow;
volatile bool responseAvailable;

#endif
