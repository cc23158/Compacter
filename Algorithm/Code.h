#ifndef CODE
#define CODE

#include "../myTypes.h"

typedef struct
{
    U8* byte;    // dynamic array of code bits
    U8 capacity; // total capacity in bits
    U8 size;     // current number of bits used
} Code;

boolean newCode(Code* code);
void freeCode(Code* code);

boolean addBit(Code* code, U8 value); // value = 1 or 0
boolean removeBit(Code* code);
boolean clone(Code original, Code* copy); // original = value ; copy = reference

#endif