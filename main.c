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

PriorityQueue* read(FILE *file);
void readFrequencies(FILE *file, U32 frequencies[256]);
PriorityQueue* createQueueFromFrequencies(U32 frequencies[256]);
NodePtr buildHuffman(PriorityQueue* pq);
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
    /* 1. Abertura do arquivo de leitura */
    FILE *file = fopen(fileName, "rb");
    if(!file) { return -1; }
    
    /* 2. Leitura das frequências dos caracteres + fila de prioridade */
    PriorityQueue* pq = read(file);
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
        fclose(fileOut);
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
    
    
    
    fclose(file);
    fclose(fileOut);
    return 0;
}

int doDecode(char *fileName)
{
    return 1;
}

PriorityQueue* read(FILE *file)
{
    U32 frequencies[256];
    readFrequencies(file, frequencies);
    return createQueueFromFrequencies(frequencies);
}

void readFrequencies(FILE *file, U32 frequencies[256])
{
    for(int i = 0; i < 256; i++) { frequencies[i] = 0; }
    U8 byte;
    
    while(fread(&byte, sizeof(U8), 1, file) == 1) { frequencies[byte]++; }
    fseek(file, 0, SEEK_SET);
}

PriorityQueue* createQueueFromFrequencies(U32 frequencies[256])
{
    PriorityQueue* pq = createQueue(256);
    if(!pq) { return NULL; }
    
    for(int i = 0; i < 256; i++)
    {
        if(frequencies[i] > 0)
        {
            U8 *ptr = malloc(sizeof(U8));
            *ptr = (U8)i;
            
            enqueue(pq, newNode(*ptr, frequencies[i]));
        }
    }
    return pq;
}

NodePtr buildHuffman(PriorityQueue* pq)
{
    if(!pq || isEmpty(pq)) { return NULL; }
    
    // fila com 1 nodo
    if(pq->size == 1)
    {
        NodePtr root = dequeue(pq);
        NodePtr newNodeTree = newNode('\0', root->frequency);
        if(!newNodeTree)
        {
            free(root);
            return NULL;
        }
        
        newNodeTree->left = root;
        enqueue(pq, newNodeTree);
    }
    
    else
    {
        while(pq->size > 1)
        {
            NodePtr node1 = dequeue(pq);
            NodePtr node2 = dequeue(pq);
            if(!node1 || !node2)
            {
                if(node1) { free(node1); }
                if(node2) { free(node2); }
                return NULL;
            }
            
            NodePtr newNodeTree = newNode('\0', node1->frequency + node2->frequency);
            if(!newNodeTree)
            {
                free(node1);
                free(node2);
                return NULL;
            }
            
            newNodeTree->left = node1;
            newNodeTree->right = node2;
    
            enqueue(pq, newNodeTree);
        }
    }
    return dequeue(pq);
}

void buildCodes(NodePtr node, Code* table[256], Code* current)
{
    if(!node) { return; }

    // se é folha, assimila o código ao caracter
    if(node->left == NULL && node->right == NULL)
    {
        Code* code = (Code*)malloc(sizeof(Code));
        if(!code) { return; }
        
        if(!clone(*current, code))
        {
            free(code);
            return;
        }
        table[node->character] = code;
        return;
    }
    
    if(!addBit(current, 0)) { return; } // vai(tenta) pra esquerda da árvore
    buildCodes(node->left, table, current);
    if(!removeBit(current)) { return; } // remove(tenta) o bit 0 para ir pra direita da árvore

    if(!addBit(current, 1)) { return; } // vai(tenta) pra direita da árvore
    buildCodes(node->right, table, current);
    if(!removeBit(current)) { return; } // remove(tenta) o bit 1 para ir pra esquerda da árvore
}