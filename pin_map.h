#ifndef PIN_DEFS_H
#define PIN_DEFS_H
/* 
>>> These are the ARDUINO pin mappings. <<<
#define US_RECV  12
#define US_ECHO  A4  
#define US_TRIG  A5 

#define H_A_EN 5
#define H_B_EN 6
#define H_IN1  7
#define H_IN2  8
#define H_IN3  9
#define H_IN4  11

#define IR_RECV 12

#define LED    13

#define LINE_R 10
#define LINE_M 4
#define LINE_L 2
*/

//<<< These are bit offsets for each pin, usable for PIN, DDR, PORT. >>>
#define US_RECV   4  // PORTB
#define US_ECHO   4  // PORTC
#define US_TRIG   5  // PORTC

// H bridge pins
#define H_A_EN    5  // PORTD PWM pin
#define H_B_EN    6  // PORTD PWM pin
#define H_IN1     7  // PORTD
#define H_IN2     0  // PORTB
#define H_IN3     1  // PORTB
#define H_IN4     3  // PORTB

#define IR_RECV   6  // PORTB

#define LED       5  // PORTB

#define LINE_R    2  // PORTB
#define LINE_M    4  // PORTD
#define LINE_L    2  // PORTD


#endif
