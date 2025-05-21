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
    // <main.c> <encode / decode> <inputFile>
    // ./Compacter.out encode teste.txt
    if(argc != 3) { return -1; }

    if(strcmp(argv[1], "encode") == 0) { return doEncode(argv[2]); }
    else if(strcmp(argv[1], "decode") == 0) { return doDecode(argv[2]); }
    else { return -1; }
}

int doEncode(char *fileName)
{
    /* 1. Abertura do arquivo de leitura */
    FILE *file = fopen(fileName, "rb");
    if(!file) { return -1; }
    
    /* 2. Leitura das frequências dos caracteres + fila de prioridade */
    U64 frequencies[256];
    PriorityQueue* pq = read(file, frequencies);
    if(!pq)
    {
        fclose(file);
        return -1;
    }

    /* 3. Criação da árvore de Huffman */
    NodePtr huffmanTree = buildHuffman(pq);
    if(!huffmanTree)
    {
        fclose(file);
        return -1;
    }
    destroyQueue(pq);
    
    /* 4. Criação de códigos para cada caractere lido */
    Code* tableOfCodes[256] = {NULL};
    Code currentCode;
    if(!newCode(&currentCode))
    {
        freeNode(huffmanTree);
        fclose(file);
        return -1;
    }
    buildCodes(huffmanTree, tableOfCodes, &currentCode);
    
    /* 4(teste). Impressão dos códigos no terminal */
    for (int b = 0; b < 256; ++b)
    {
        Code *code = tableOfCodes[b];
        if (!code) { continue; }
    
        if (b >= 32 && b <= 126) { printf(" '%c' | ", (char)b); } 
        else { printf("\\x%02X | ", b); }
    
        /* ---- imprime os bits ---- */
        for (int i = 0; i < code->size; ++i)
        {
            int realPos = (code->capacity - code->size) + i;
            int byteIdx = realPos / 8;
            int bitIdx  = 7 - (realPos & 7);
            U8 bit = (code->byte[byteIdx] >> bitIdx) & 1;
            putchar(bit ? '1' : '0');
        }
        putchar('\n');
    }
    
    /* 5. Criação do arquivo de gravação */
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
    
    /* 6. Escrita do cabeçalho */
    fileWrite(fileOut, frequencies);
    
    encode(file, fileOut, huffmanTree, tableOfCodes);
    
    fclose(file);
    fclose(fileOut);
    return 0;
}

int doDecode(char *fileName)
{
    FILE *file = fopen(fileName, "rb");
    if (!file) { return -1; }

    /* 1. Leitura do cabeçalho */
    U64 frequencies[256];
    fileRead(file, frequencies);

    /* 2. Reconstrução da árvore de Huffman */
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

    decode(file, huffmanTree);

    fclose(file);
    freeNode(huffmanTree);
    return 0;
}