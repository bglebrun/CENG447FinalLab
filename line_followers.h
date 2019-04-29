#ifndef _LINE_FOLLOWERS_H_
#define _LINE_FOLLOWERS_H_
#include "bit_macros.h"
#include "pin_map.h"
#include <stdbool.h>
#include <avr/io.h>

void init_line_followers();
bool line_left();
bool line_right();
bool line_mid();
bool all_sensor();

#endif