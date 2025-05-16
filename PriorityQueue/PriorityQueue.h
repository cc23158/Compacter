#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include "../myTypes.h"

typedef struct Node {
    U8 character;  // caracter
    U64 frequency; // quantidades de aparições
    struct Node* left;
    struct Node* right;
} Node;

typedef Node* NodePtr; // ponteiro para um nodo

typedef struct
{
    NodePtr *data;
    U64 size;     // capacidade real
    U64 capacity; // capacidade total
} PriorityQueue;

NodePtr newNode(U8 character, U64 frequency);

PriorityQueue* createQueue(U64 capacity);
void destroyQueue(PriorityQueue* pq);

boolean isEmpty(PriorityQueue* pq);
boolean isFull(PriorityQueue* pq);

void enqueue(PriorityQueue* pq, NodePtr node);
NodePtr dequeue(PriorityQueue* pq);

#endif