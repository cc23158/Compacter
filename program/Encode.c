#include <stdio.h>
#include <string.h>
#include "Encode.h"

void encode(FILE *inputFile, FILE *outputFile, NodePtr huffmanTree, Code *codeTable[256])
{
    if(!inputFile || !outputFile || !huffmanTree || !codeTable) { return; }

    /* 7. Codificação do arquivo */
    U8 byte;
    U8 currentByte = 0;
    int bitCount = 0;
    int lastByteBits = 0; // Para salvar no final

    while (fread(&byte, sizeof(U8), 1, inputFile) == 1)
    {
        Code *code = codeTable[byte];
        for (int i = 0; i < code->size; ++i)
        {
            int byteIdx = i / 8;
            int bitIdx  = 7 - (i % 8); // MSB-first
    
            U8 bit = (code->byte[byteIdx] >> bitIdx) & 1;
    
            currentByte <<= 1;
            currentByte |= bit;
            bitCount++;
    
            if (bitCount == 8)
            {
                fwrite(&currentByte, sizeof(U8), 1, outputFile);
                bitCount = 0;
                currentByte = 0;
            }
        }
    }


    // Último byte incompleto
    if (bitCount > 0)
    {
        currentByte <<= (8 - bitCount); // preenche com zeros à direita
        fwrite(&currentByte, sizeof(U8), 1, outputFile);
        lastByteBits = bitCount;
    }
    else { lastByteBits = 8; }

    // Escreve no final o número de bits úteis no último byte (1 byte extra)
    fwrite(&lastByteBits, sizeof(U8), 1, outputFile);
}