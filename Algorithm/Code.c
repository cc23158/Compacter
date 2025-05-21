#include <stdlib.h>
#include "Code.h"

// cria novo código
boolean newCode(Code* code)
{
    // aloca memória para 1 unidade de byte, 8 bits
    code->byte = (U8*)malloc(1 * sizeof(U8));
    if(code->byte == NULL) { return false; }

    code->capacity = 8; // 8 bits
    code->size = 0;     // tamanho do código
    return true;
}

void freeCode(Code* code)
{
    // libera a memória do byte
    if(code->byte != NULL) { free(code->byte); }
    code->byte = NULL;
}

boolean addBit(Code* code, U8 value)
{
    if (code->size == code->capacity)
    {
        int newByteCount = (code->capacity + 8) / 8;
        U8* newBytes = (U8*)malloc(newByteCount);
        if (!newBytes) return false;

        // Copia os bytes antigos
        for (int i = 0; i < code->capacity / 8; i++) {
            newBytes[i] = code->byte[i];
        }
        // Zera o novo byte (caso tenha sido adicionado)
        if (newByteCount > code->capacity / 8) {
            newBytes[newByteCount - 1] = 0;
        }

        free(code->byte);
        code->byte = newBytes;
        code->capacity += 8;
    }

    int byteIndex = code->size / 8;
    int bitOffset = 7 - (code->size % 8);  // MSB-first

    if (value == 1) {
        code->byte[byteIndex] |= (1 << bitOffset);
    } else {
        code->byte[byteIndex] &= ~(1 << bitOffset);
    }

    code->size++;
    return true;
}


boolean removeBit(Code* code)
{
    if (code->size == 0) return false;
    code->size--;
    return true;
}


boolean clone(Code original, Code* copy)
{
    int byteCount = (original.size + 7) / 8; // total de bytes usados
    copy->byte = (U8*)malloc(byteCount);
    if (!copy->byte) return false;

    for (int i = 0; i < byteCount; i++) {
        copy->byte[i] = original.byte[i];
    }

    copy->capacity = byteCount * 8;
    copy->size = original.size;
    return true;
}