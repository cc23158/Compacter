#ifndef CODE
#define CODE

#include "../myTypes.h"

typedef struct
{
    U8* byte;    // array dinâmico do código do char
    U8 capacity; // em bits
    U8 size;     // em bits
} Code;

boolean newCode(Code* code);
void freeCode(Code* code);

boolean addBit(Code* code, U8 value); // value = 1 or 0
boolean removeBit(Code* code);
boolean clone(Code original, Code* copy); // original = value ; copy = reference

#endif