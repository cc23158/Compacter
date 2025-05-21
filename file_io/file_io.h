#ifndef FILE_IO
#define FILE_IO

#include <stdio.h>
#include "../myTypes.h"
#include "../queue/PriorityQueue.h"

PriorityQueue* read(FILE* file, U64 frequencies[256]);
void readFrequencies(FILE *file, U64 frequencies[256]);
void fileReader(FILE *file, U64 frequencies[256]);
void fileWriter(FILE *file, U64 frequencies[256]);

#endif