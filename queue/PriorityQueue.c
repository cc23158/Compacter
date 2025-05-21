#include <stdlib.h>
#include "PriorityQueue.h"

static void swap(NodePtr* a, NodePtr* b);

// create new node
NodePtr newNode(U8 character, U64 frequency)
{
    // allocate memory for the new node
    NodePtr new = (NodePtr)malloc(sizeof(Node));
    if(!new) { return NULL; }

    new->character = character;
    new->frequency = frequency;
    new->left = NULL;
    new->right = NULL;
    return new;
}

// free the memory of a node
void freeNode(NodePtr node)
{
    if(!node) { return; }
    
    // free memory of its subtrees before itself
    freeNode(node->left);
    freeNode(node->right);
    free(node);
}

// create a priority queue
PriorityQueue* createQueue(U64 capacity)
{
    // allocate memory for the priority queue
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if(!pq) { return NULL; }

    // allocate memory to queue's array of <capacity> node pointers
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

// free the priority queue
void destroyQueue(PriorityQueue* pq)
{
    if(pq)
    {
        free(pq->data);
        free(pq);
    }
}

// return whether the queue is empty
boolean isEmpty(PriorityQueue* pq) { return pq->size == 0; }

// return whether the queue is full
boolean isFull(PriorityQueue* pq) { return pq->size == pq->capacity; }

// add a new node to the queue
void enqueue(PriorityQueue* pq, NodePtr node)
{
    // if the queue is full, cannot add more nodes
    if(isFull(pq)) { return; }

    // add node at the last position
    U64 i = pq->size++;
    pq->data[i] = node;
    
    // bubble sort by frequency
    while(i > 0 && pq->data[i]->frequency < pq->data[i - 1]->frequency)
    {
        swap(&pq->data[i], &pq->data[i - 1]);
        i--;
    }
}

// remove node with highest priority (<<frequency)
NodePtr dequeue(PriorityQueue* pq)
{
    // if the queue is empty, return NULL
    if(!pq->size) { return NULL; }

    NodePtr item = pq->data[0];

    // shift remaining nodes to the left
    for(U64 i = 1; i < pq->size; i++) { pq->data[i - 1] = pq->data[i]; }
    pq->size--;
    
    return item;
}

// swap 2 node pointers
static void swap(NodePtr* a, NodePtr* b)
{
    NodePtr temp = *a;
    *a = *b;
    *b = temp;
}