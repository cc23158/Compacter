#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "myTypes.h"
#include "queue/PriorityQueue.h"
#include "algorithm/Code.h"
#include "algorithm/Huffman.h"
#include "file_io/file_io.h"
#include "program/Encode.h"
#include "program/Decode.h"

int doEncode(char *fileName);
int doDecode(char *fileName);

int main(int argc, char *argv[])
{
    if(argc != 3) { return -1; }

    if(strcmp(argv[1], "encode") == 0) { return doEncode(argv[2]); }
    else if(strcmp(argv[1], "decode") == 0) { return doDecode(argv[2]); }
    else { return -1; }
}

int doEncode(char *fileName)
{
    FILE *file = fopen(fileName, "rb");
    if(!file) { return -1; }

    U64 frequencies[256];
    PriorityQueue* pq = read(file, frequencies);
    if(!pq)
    {
        fclose(file);
        return -1;
    }

    NodePtr huffmanTree = buildHuffman(pq);
    if(!huffmanTree)
    {
        fclose(file);
        return -1;
    }
    destroyQueue(pq);

    Code* tableOfCodes[256] = {NULL};
    Code currentCode;
    if(!newCode(&currentCode))
    {
        freeNode(huffmanTree);
        fclose(file);
        return -1;
    }
    buildCodes(huffmanTree, tableOfCodes, &currentCode);

    const char *output = "compressed.huff";
    FILE *fileOut = fopen(output, "wb");
    if(!fileOut)
    {
        fclose(file);
        freeNode(huffmanTree);
        for(int i = 0; i < 256; ++i)
        {
            if(tableOfCodes[i])
            {
                freeCode(tableOfCodes[i]);
                free(tableOfCodes[i]);
            }
        }
        return -1;
    }

    // >>> Adiciona extensão original ao início do .huff
    const char *ext = strrchr(fileName, '.');
    if (!ext) { ext = ""; } else { ext++; }
    U8 extLen = (U8)strlen(ext);
    fwrite(&extLen, sizeof(U8), 1, fileOut);
    fwrite(ext, sizeof(char), extLen, fileOut);

    // Escrita do cabeçalho e dados
    fileWrite(fileOut, frequencies);
    encode(file, fileOut, huffmanTree, tableOfCodes);

    fclose(file);
    fclose(fileOut);
    freeNode(huffmanTree);
    for (int i = 0; i < 256; ++i)
    {
        if (tableOfCodes[i])
        {
            freeCode(tableOfCodes[i]);
            free(tableOfCodes[i]);
        }
    }

    return 0;
}

int doDecode(char *fileName)
{
    FILE *file = fopen(fileName, "rb");
    if (!file) { return -1; }

    // <<< Leitura da extensão
    U8 extLen;
    fread(&extLen, sizeof(U8), 1, file);

    char ext[256] = {0};
    fread(ext, sizeof(char), extLen, file);

    // <<< Criação do nome do arquivo de saída
    char outFileName[300];
    snprintf(outFileName, sizeof(outFileName), "decompressed.%s", ext);

    // <<< Leitura do cabeçalho
    U64 frequencies[256];
    fread(frequencies, sizeof(U64), 256, file);

    PriorityQueue *pq = createQueueFromFrequencies(frequencies);
    if (!pq)
    {
        fclose(file);
        return -1;
    }

    NodePtr huffmanTree = buildHuffman(pq);
    destroyQueue(pq);
    if (!huffmanTree)
    {
        fclose(file);
        return -1;
    }

    decode(file, huffmanTree, outFileName);

    fclose(file);
    freeNode(huffmanTree);
    return 0;
}
