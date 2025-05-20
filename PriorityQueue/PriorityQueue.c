#include <stdlib.h>
#include "PriorityQueue.h"

static void swap(NodePtr* a, NodePtr* b);

// cria um novo nodo
NodePtr newNode(U8 character, U64 frequency)
{
    // aloca memória para o novo nodo
    NodePtr new = (NodePtr)malloc(sizeof(Node));
    if(!new) { return NULL; } // verifica se a memória do nodo foi alocada com sucesso

    new->character = character;
    new->frequency = frequency;
    new->left = NULL;
    new->right = NULL;
    return new;
}

void freeNode(NodePtr node)
{
    if(!node) { return; }
    
    freeNode(node->left);
    freeNode(node->right);
    free(node);
}

PriorityQueue* createQueue(U64 capacity)
{
    // aloca memória para a fila de prioridade e verifica
    // se a memória foi alocada com sucesso
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if(!pq) { return NULL; }

    // aloca memória para o array de ponteiros de nodos baseado na capacidade dada
    pq->data = (NodePtr*)malloc(sizeof(NodePtr) * capacity);
    if(!pq->data)
    {
        free(pq);
        return NULL;
    }

    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

// destrói a fila de prioridade
void destroyQueue(PriorityQueue* pq)
{
    if(pq)
    {
        free(pq->data);
        free(pq);
    }
}

// retorna se está vazia
boolean isEmpty(PriorityQueue* pq) { return pq->size == 0; }

// retorna se está cheia
boolean isFull(PriorityQueue* pq) { return pq->size == pq->capacity; }

// insere um elemento na fila de prioridade
void enqueue(PriorityQueue* pq, NodePtr node)
{
    // se a fila está cheia, não se pode inserir elementos
    if(isFull(pq)) { return; }

    U32 i = pq->size++;
    pq->data[i] = node;
    
    while(i > 0 && pq->data[i]->frequency < pq->data[i - 1]->frequency)
    {
        swap(&pq->data[i], &pq->data[i - 1]);
        i--;
    }
}

NodePtr dequeue(PriorityQueue* pq)
{
    if(!pq->size) { return NULL; }

    NodePtr item = pq->data[0]; // armazena o elemento com menor frequência
    for(U32 i = 1; i < pq->size; i++) { pq->data[i - 1] = pq->data[i]; }
    pq->size--;
    
    return item; // first-in, first-out
}

// troca os ponteiros de 2 nodos
static void swap(NodePtr* a, NodePtr* b)
{
    NodePtr temp = *a;
    *a = *b;
    *b = temp;
}