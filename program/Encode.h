#ifndef ENCODE
#define ENCODE

#include <stdio.h>
#include "../myTypes.h"
#include "../queue/PriorityQueue.h"
#include "../algorithm/Code.h"
#include "../algorithm/Huffman.h"
#include "../file_io/file_io.h"

void encode(FILE *inputFile, FILE *outputFile, NodePtr huffmanTree, Code *codeTable[256]);

#endif