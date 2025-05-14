// https://www.geeksforgeeks.org/c-program-to-implement-priority-queue/

#include <stdlib.h>
#include "PriorityQueue.h"

static void swap(NodePtr* a, NodePtr* b);
static void heapifyUp(PriorityQueue* pq, U8 index);
static void heapifyDown(PriorityQueue* pq, U8 index);

// troca os ponteiros de 2 nodos
static void swap(NodePtr* a, NodePtr* b)
{
    NodePtr temp = *a;
    *a = *b;
    *b = temp;
}

PriorityQueue* createQueue(U8 capacity)
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
    if (pq)
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
    if (isFull(pq)) { return; }

    pq->data[pq->size++] = node;
    heapifyUp(pq, pq->size - 1);
}

static void heapifyUp(PriorityQueue* pq, U8 index)
{
    // verifica o limite da fila e se
    // é necessário trocar os nodos de posição
    if(index != 0 && pq->data[(index - 1) / 2]->frequency > pq->data[index]->frequency)
    {
        swap(&pq->data[(index - 1) / 2], &pq->data[index]);
        heapifyUp(pq, (index - 1) / 2)
    }
}

static void heapifyDown(PriorityQueue* pq, U8 index)
{
    U8 smallest = index;
    U8 left = 2 * index + 1;
    U8 right = 2 * index + 2;

    if (left < pq->size && pq->data[left]->frequency < pq->data[smallest]->frequency)
        smallest = left;

    if (right < pq->size && pq->data[right]->frequency < pq->data[smallest]->frequency)
        smallest = right;

    if (smallest != index) {
        swap(&pq->data[index], &pq->data[smallest]);
        heapifyDown(pq, smallest);
    }
}

NodePtr dequeue(PriorityQueue* pq)
{
    if(!pq->size) { return NULL; }

    U8 item = pq->data[0];
    pq->data[0] = pq->data[--pq->size];
    heapifyDown(pq, 0);
    return item;
}