#include "line_followers.h"

/*
 * Right sensor is the only 
 * pin that sits on PORTB, 
 * all others are on PORTD
 */ 

void init_line_followers() {
    clearBit(DDRB, LINE_R);
    clearBit(DDRD, LINE_M);
    clearBit(DDRD, LINE_L);
}

bool line_right() {
    return ~(bool)(PINB & bitVal(LINE_R)); 
}

bool line_left() {
    return ~(bool)(PIND & bitVal(LINE_L));
}

bool line_mid() {
    return ~(bool)(PIND & bitVal(LINE_M)); 
}

bool all_sensor() {
    return (line_left() && line_right() && line_mid());
}