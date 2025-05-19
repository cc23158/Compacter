/*
Num de bits para considere no último byte.
Num de bytes diferentes na tabela de frequência.
Byte -> Frequência
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "myTypes.h"
#include "PriorityQueue/PriorityQueue.h"
#include "Algorithm/Code.h"

int doEncode(char *fileName);
int doDecode(char *fileName);

PriorityQueue* read(const char *fileName);
void readFrequencies(const char *fileName, U32 frequencies[256]);
PriorityQueue* createQueueFromFrequencies(U32 frequencies[256]);
void buildCodes(NodePtr node, Code* table[256], Code* current);

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
    PriorityQueue* pq = read(fileName);
    if(!pq) { return -1; }

    while (pq->size > 1)
    {
        NodePtr node1 = dequeue(pq);
        NodePtr node2 = dequeue(pq);
        
        NodePtr newNodeTree = newNode('\0', node1->frequency + node2->frequency);
        newNodeTree->left = node1;
        newNodeTree->right = node2;

        enqueue(pq, newNodeTree);
    }
    
    Code* tableOfCodes[256] = {NULL};
    Code currentCode;
    newCode(&currentCode);

    buildCodes(dequeue(pq), tableOfCodes, &currentCode);
        
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
    
    destroyQueue(pq);
    return 0;
}

int doDecode(char *fileName)
{
    return 1;
}

PriorityQueue* read(const char *fileName)
{
    U32 frequencies[256];
    readFrequencies(fileName, frequencies);
    return createQueueFromFrequencies(frequencies);
}

void readFrequencies(const char *fileName, U32 frequencies[256])
{
    FILE *file = fopen(fileName, "rb+");
    if (!file) { exit(1); }
    
    for (int i = 0; i < 256; i++) { frequencies[i] = 0; }
    U8 byte;
    
    while (fread(&byte, sizeof(U8), 1, file) == 1) { frequencies[byte]++; }
    fclose(file);
}

PriorityQueue* createQueueFromFrequencies(U32 frequencies[256])
{
    PriorityQueue* pq = createQueue(256);
    if (!pq) { return NULL; }
    
    for (int i = 0; i < 256; i++)
    {
        if (frequencies[i] > 0)
        {
            U8 *ptr = malloc(sizeof(U8));
            *ptr = (U8)i;
            
            enqueue(pq, newNode(*ptr, frequencies[i]));
        }
    }
    
    return pq;
}

void buildCodes(NodePtr node, Code* table[256], Code* current)
{
    if (!node) { return; }

    // se é folha, assimila o código ao caracter
    if (node->left == NULL && node->right == NULL)
    {
        Code* code = (Code*)malloc(sizeof(Code));
        clone(*current, code);
        table[node->character] = code;
        return;
    }
    
    addBit(current, 0); // vai para a esquerda da árvore
    buildCodes(node->left, table, current);
    removeBit(current); // remove o bit 0 para ir para a direita da árvore

    addBit(current, 1);
    buildCodes(node->right, table, current);
    removeBit(current); // remove o bit 1 para ir para a esquerda da árvore
}