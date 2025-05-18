#include <stdlib.h>
#include "Code.h"

// cria novo código
boolean newCode(Code* code)
{
    // aloca memória para 1 unidade de byte, 8 bits
    code->byte = (U8*)malloc(1 * sizeof(U8));
    if (code->byte == NULL) { return false; }

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
    // se chegou a capacidade máxima de bits, expande o array
    if(code->size == code->capacity)
    {
        // aloca na memória 1 byte a mais
        U8* new = (U8*)malloc((code->capacity / 8 + 1) * sizeof(U8));
        if (new == NULL) { return false; }

        for(int i = 0; i < code->size / 8; i++) { new[i] = code->byte[i]; }

        free(code->byte);
        code->byte = new;

        code->capacity += 8;
    }

    // joga 1 posição a esquerda e coloca 0 na direita
    code->byte[code->capacity / 8 - 1] <<= 1;

    if (value == 1) { code->byte[code->capacity / 8 - 1] |= 1; }

    code->size++;
    return true;
}

boolean removeBit(Code* code)
{
    // se não há código, não pode remover bit
    if (code->size == 0) { return false; }

    // joga 1 posição a direita e coloca 0 na esquerda
    code->byte[code->capacity / 8 - 1] >>= 1;
    code->size--;

    if (code->capacity > 8 && code->capacity - code->size == 8)
    {
        U8* new = (U8*)malloc((code->capacity / 8 - 1) * sizeof(U8));
        if (new == NULL) { return false; }

        for(int i = 0; i < code->size / 8; i++) { new[i] = code->byte[i]; }

        free(code->byte);
        code->byte = new;

        code->capacity -= 8;
    }
    return true;
}

boolean clone(Code original, Code* copy)
{
    copy->byte = (U8*)malloc(original.capacity * sizeof(U8));
    if (copy->byte == NULL) { return false; }

    for (int i = 0; i < original.capacity / 8; i++) { copy->byte[i] = original.byte[i]; }
    copy->capacity = original.capacity;
    copy->size = original.size;
    return true;
}