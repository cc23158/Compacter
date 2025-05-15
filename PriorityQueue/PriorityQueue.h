#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include "../myTypes.h"
#include "../BinaryTree/BinaryTree.h"

typedef void* element;
typedef I8 (*CompareFunc)(element a, element b);

typedef struct
{
    NodePtr *data;
    U64 size;
    U64 capacity;
} PriorityQueue;

PriorityQueue* createQueue(U64 capacity);
void destroyQueue(PriorityQueue* pq);
boolean isEmpty(PriorityQueue* pq);
boolean isFull(PriorityQueue* pq);
void enqueue(PriorityQueue* pq, NodePtr node);
NodePtr dequeue(PriorityQueue* pq);

#endif