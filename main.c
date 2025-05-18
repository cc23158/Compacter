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

void read(const char *fileName, PriorityQueue* pq);
void buildCodes(NodePtr node, Code* table[256], Code* current);

int main()
{
    // criar fila e árvore
    PriorityQueue* pq = createQueue(256);
    
    // verifica se a fila foi criada corretamente
    if(!pq) { return 1; }

    read("teste.txt", pq);
    
    // construção da árvore de Huffman
    while (pq->size > 1)
    {
        NodePtr node1 = dequeue(pq);
        NodePtr node2 = dequeue(pq);
        
        // cria novo nodo de árvore com os dois primeiros da fila
        // e atualiza a árvore e a fila
        NodePtr newNodeTree = newNode('\0', node1->frequency + node2->frequency);
        newNodeTree->left = node1;
        newNodeTree->right = node2;

        enqueue(pq, newNodeTree);
    }
    
    Code* tableOfCodes[256] = { NULL };
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

void read(const char *fileName, PriorityQueue* pq)
{
    FILE *file = fopen(fileName, "rb+"); // leitura em modo binário

    // se o arquivo não foi aberto com sucesso, não pode ler
    if (file == NULL) { exit(1); }

    // vetor de U8
    U8 frequencies[256] = {0};

    U8 byte;
    // fseek(f, deslocamento, ptReferencia)
    // SEEK_SET, SEEK_END, SEEK_CUR
    // fread -> retorna um inteiro com a quantidade de bytes lidos
    
    // faz a leitura de 1 byte / char do arquivo file,
    // armazena o char lido no endereço da variável byte
    // e aumenta a frequência desse char no array
    while (fread(&byte, sizeof(U8), 1, file) == 1) { frequencies[byte]++; }

    // fechar arquivo de leitura
    fclose(file);

    for (int i = 0; i < 256; i++)
    {
        // verifica se o caracter apareceu no arquivo lido
        if (frequencies[i] > 0)
        {
            U8 *ptr = malloc(sizeof(U8));
            *ptr = (U8)i; // ponteiro para o char

            // cria nodo e adiciona na fila
            NodePtr node = newNode(*ptr, frequencies[i]);
            enqueue(pq, node);
        }
    }
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