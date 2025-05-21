#ifndef CODE
#define CODE

#include "../myTypes.h"

typedef struct
{
    U8* byte;    // dynamic array of code
    U8 capacity; // bits
    U8 size;     // bits
} Code;

boolean newCode(Code* code);
void freeCode(Code* code);

boolean addBit(Code* code, U8 value); // value = 1 or 0
boolean removeBit(Code* code);
boolean clone(Code original, Code* copy); // original = value ; copy = reference

#endif