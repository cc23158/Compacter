#include <stdlib.h>
#include "file_io.h"
#include "../algorithm/Huffman.h"

PriorityQueue* read(FILE *file, U64 frequencies[256])
{
    readFrequencies(file, frequencies);
    return createQueueFromFrequencies(frequencies);
}

void readFrequencies(FILE *file, U64 frequencies[256])
{
    for (int i = 0; i < 256; i++) { frequencies[i] = 0; }

    U8 byte;
    while (fread(&byte, sizeof(U8), 1, file) == 1)
    {
        frequencies[byte]++;
    }

    fseek(file, 0, SEEK_SET); // volta ao início
}

void fileRead(FILE *file, U64 frequencies[256]) { fread(frequencies, sizeof(U64), 256, file); }

void fileWrite(FILE *file, U64 frequencies[256]) { fwrite(frequencies, sizeof(U64), 256, file); }