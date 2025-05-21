#ifndef DECODE
#define DECODE

#include <stdio.h>
#include "../myTypes.h"
#include "../queue/PriorityQueue.h"
#include "../algorithm/Huffman.h"

void decode(FILE *inputFile, NodePtr huffmanTree);

#endif