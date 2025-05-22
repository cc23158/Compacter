#ifndef PRIORITY_QUEUE
#define PRIORITY_QUEUE

#include "../myTypes.h"

typedef struct Node
{
    U8 character;  // character
    U64 frequency; // number of appearances
    struct Node* left;
    struct Node* right;
} Node;

typedef Node* NodePtr; // node pointer

typedef struct
{
    NodePtr *data;
    U64 size;     // number of elements
    U64 capacity; // total capacity
} PriorityQueue;

NodePtr newNode(U8 character, U64 frequency);
void freeNode(NodePtr node);

PriorityQueue* createQueue(U64 capacity);
void destroyQueue(PriorityQueue* pq);

boolean isEmpty(PriorityQueue* pq);
boolean isFull(PriorityQueue* pq);

void enqueue(PriorityQueue* pq, NodePtr node);
NodePtr dequeue(PriorityQueue* pq);

#endif