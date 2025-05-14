#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include "myTypes.h"

typedef *void element;
typedef I8 (*CompareFunc)(element a, element b);

typedef struct Node {
    element info;
    U8 frequency;
    struct Node* left;
    struct Node* right;
} Node;

typedef Node* NodePtr;

typedef struct
{
    NodePtr *data;
    U8 size;
    U8 capacity;
} PriorityQueue;

PriorityQueue* createQueue(U8 capacity);
void destroyQueue(PriorityQueue* pq);
boolean isEmpty(PriorityQueue* pq);
boolean isFull(PriorityQueue* pq);
void enqueue(PriorityQueue* pq, NodePtr node);
NodePtr extractMin(PriorityQueue* pq);

#endif