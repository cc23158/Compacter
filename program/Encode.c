#include <stdio.h>
#include <string.h>
#include "Encode.h"

void encode(FILE *inputFile, FILE *outputFile, NodePtr huffmanTree, Code *codeTable[256])
{
    if(!inputFile || !outputFile || !huffmanTree || !codeTable) { return; }

    U8 byte;
    U8 currentByte = 0;
    int bitCount = 0;
    int lastByteBits = 0;

    // 1. read each byte from the original file
    // 2. find the code of the read character
    // 3. write the bits of the code in the file
    while(fread(&byte, sizeof(U8), 1, inputFile) == 1)
    {
        Code *code = codeTable[byte];
        for(int i = 0; i < code->size; ++i)
        {
            // byte index in code->byte[x]
            int byteIdx = i / 8;

            // bit index in code->byte[byteIdx] = (0 - 7)
            int bitIdx  = 7 - (i % 8);
    
            // get the bit to write from the code. Ex: bit 5
            // 01010101 -> (move 5 bits to right) -> 00000010 -> get last bit (0)
            U8 bit = (code->byte[byteIdx] >> bitIdx) & 1;
    
            currentByte <<= 1;  // move all bits 1 position to left (Ex: 01010101 -> 10101010)
            currentByte |= bit; // add the new bit in the last position (Ex: 10101010 -> 10101011)
            bitCount++;
    
            // if is a full byte has been constructed, write it to the output file
            if(bitCount == 8)
            {
                fwrite(&currentByte, sizeof(U8), 1, outputFile);
                bitCount = 0;
                currentByte = 0;
            }
        }
    }

    // last byte incomplete
    if(bitCount > 0)
    {
        currentByte <<= (8 - bitCount);                  // fill with 0 on the right
        fwrite(&currentByte, sizeof(U8), 1, outputFile); // write the byte
        lastByteBits = bitCount;                         // how many valid bits in the last byte
    }

    // last byte complete, 8 bits = 1 byte
    else { lastByteBits = 8; }

    // at the end of the file, write how many bits to consider in the last byte
    fwrite(&lastByteBits, sizeof(U8), 1, outputFile);
}