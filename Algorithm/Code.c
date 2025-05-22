#include <stdlib.h>
#include "Code.h"

// create new code
boolean newCode(Code* code)
{
    // allocate memory for 8 bits = 1 byte
    code->byte = (U8*)malloc(1 * sizeof(U8));
    if(code->byte == NULL) { return false; }

    code->capacity = 8; // 8 bits
    code->size = 0;     // code size
    return true;
}

// free memory used by the code
void freeCode(Code* code)
{
    if(code->byte != NULL) { free(code->byte); }
    code->byte = NULL;
}

// add a bit to the code
boolean addBit(Code* code, U8 value)
{
    // if the code needs to be expanded
    if(code->size == code->capacity)
    {
        // allocate memory for +1 byte
        int newByteCount = (code->capacity + 8) / 8;
        U8* newBytes = (U8*)malloc(newByteCount);
        if(!newBytes) return false;

        // copy old content to newly allocated memory
        for(int i = 0; i < code->capacity / 8; i++) { newBytes[i] = code->byte[i]; }

        // new byte's value = 00000000
        if(newByteCount > code->capacity / 8) { newBytes[newByteCount - 1] = 0; }

        // free the old memory
        free(code->byte);
        code->byte = newBytes;
        code->capacity += 8;
    }

    // byte index to insert bit
    int byteIndex = code->size / 8;

    // bit position in byte (0 - 7)
    int bitOffset = 7 - (code->size % 8);

    // if newBit = 1 -> set bit of bitOffset position - 1 = 1 (Ex bitOffset = 3: 10101010 -> 10111010)
    // if newBit = 0 -> set bit of bitOffset position - 1 = 0 (Ex bitOffset = 3: 11111111 -> 11101111)
    if(value == 1) { code->byte[byteIndex] |= (1 << bitOffset);}
    else { code->byte[byteIndex] &= ~(1 << bitOffset); }

    code->size++;
    return true;
}

// remove 1 bit from the code
boolean removeBit(Code* code)
{
    if(code->size == 0) return false;
    code->size--;
    return true;
}

// clone the contents of the original code into the copy
boolean clone(Code original, Code* copy)
{
    int byteCount = (original.size + 7) / 8; // total bytes in use
    copy->byte = (U8*)malloc(byteCount);     // allocate the exact number of used bytes
    if(!copy->byte) return false;

    // copy the contents of original code to the copy
    for(int i = 0; i < byteCount; i++) { copy->byte[i] = original.byte[i]; }

    copy->capacity = byteCount * 8;
    copy->size = original.size;
    return true;
}