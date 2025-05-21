#ifndef HUFFMAN
#define HUFFMAN

#include "../myTypes.h"
#include "../queue/PriorityQueue.h"
#include "Code.h"

PriorityQueue* createQueueFromFrequencies(U64 frequencies[256]);
NodePtr buildHuffman(PriorityQueue* pq);
void buildCodes(NodePtr node, Code* table[256], Code* current);

#endif