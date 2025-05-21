/*
Num de bits para considere no último byte.
Num de bytes diferentes na tabela de frequência.
Byte -> Frequência
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myTypes.h"
#include "queue/PriorityQueue.h"
#include "algorithm/Code.h"
#include "algorithm/Huffman.h"
#include "file_io/file_io.h"


int doEncode(char *fileName);
int doDecode(char *fileName);

PriorityQueue* read(FILE *file, U64 frequencies[256]);
void readFrequencies(FILE *file, U64 frequencies[256]);
PriorityQueue* createQueueFromFrequencies(U64 frequencies[256]);
NodePtr buildHuffman(PriorityQueue* pq);
void buildCodes(NodePtr node, Code* table[256], Code* current);

void fileWrite(FILE *file, U64 frequencies[256]);
void fileRead(FILE *file, U64 frequencies[256]);

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
    
    /* 7. Codificação do arquivo */
    U8 byte;
    U8 currentByte = 0;
    int bitCount = 0;
    int lastByteBits = 0; // Para salvar no final

    while (fread(&byte, sizeof(U8), 1, file) == 1)
    {
        Code *code = tableOfCodes[byte];
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
                fwrite(&currentByte, sizeof(U8), 1, fileOut);
                bitCount = 0;
                currentByte = 0;
            }
        }
    }


    // Último byte incompleto
    if (bitCount > 0)
    {
        currentByte <<= (8 - bitCount); // preenche com zeros à direita
        fwrite(&currentByte, sizeof(U8), 1, fileOut);
        lastByteBits = bitCount;
    }
    else
    {
        lastByteBits = 8;
    }

    // Escreve no final o número de bits úteis no último byte (1 byte extra)
    fwrite(&lastByteBits, sizeof(U8), 1, fileOut);
    
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

    /* 3. Pega tamanho do arquivo e número de bits válidos no último byte */
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, -1, SEEK_END);
    U8 lastByteBits;
    fread(&lastByteBits, sizeof(U8), 1, file);

    long totalBytes = fileSize - 2048 - 1; // tira cabeçalho (2048) e último byte extra

    fseek(file, 2048, SEEK_SET); // vai direto pro início dos dados comprimidos

    /* 4. Decodificação dos bits */
    NodePtr current = huffmanTree;
    FILE *fileOut = fopen("decompressed.txt", "wb");
    if (!fileOut)
    {
        fclose(file);
        freeNode(huffmanTree);
        return -1;
    }

    for (long i = 0; i < totalBytes; ++i)
    {
        U8 byte;
        fread(&byte, sizeof(U8), 1, file);

        int bitsToRead = (i == totalBytes - 1) ? lastByteBits : 8;

        for (int b = 7; b >= 8 - bitsToRead; --b)
        {
            U8 bit = (byte >> b) & 1;
            current = bit ? current->right : current->left;

            if (current->left == NULL && current->right == NULL)
            {
                fwrite(&current->character, sizeof(U8), 1, fileOut);
                current = huffmanTree;
            }
        }
    }

    fclose(file);
    fclose(fileOut);
    freeNode(huffmanTree);
    return 0;
}