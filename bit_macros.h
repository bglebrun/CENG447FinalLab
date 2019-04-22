#ifndef _BIT_MACROS_H_
#define _BIT_MACROS_H

#define bitVal(bit) (1 << bit)
#define setBit(byte, bit) ((byte) |=(bitVal(bit)))
#define clearBit(byte, bit) ((byte) &= ~(bitVal(bit)))
#define togBit(byte, bit) ((byte) ^= (bitVal(bit)))

#endif