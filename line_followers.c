#include "line_followers.h"

void init_line_followers() {
    clearBit(DDRB, LINE_R);
    clearBit(DDRD, LINE_M);
    clearBit(DDRD, LINE_L);
}

bool line_left() {
    return (bool)(PORTB && bitVal(LINE_L));
}

bool line_right() {
    return (bool)(PORTD && bitVal(LINE_R)); 
}

bool line_mid() {
    return (bool)(PORTD && bitVal(LINE_M)); 
}

bool all_sensor() {
    return (bool)(line_left() && line_right() && line_mid());
}