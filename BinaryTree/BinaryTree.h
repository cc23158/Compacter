#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include "../myTypes.h"

typedef void* element;

typedef struct NodeT {
    element info;
    U64 frequency;
    struct NodeT* left;
    struct NodeT* right;
} NodeT;

typedef NodeT* NodePtr;
typedef I8 (*CompareFunc)(element a, element b);

typedef struct {
    NodePtr root;
    CompareFunc comp;
} BinaryTree;

void newTree(BinaryTree* bt, CompareFunc comp);
NodePtr newNode(element e, U64 freq);
void addNode(BinaryTree* bt, element e, U64 freq);
boolean removeNode(BinaryTree* bt, element e);
boolean existNode(BinaryTree* bt, element e);
NodePtr mergeNodes(NodePtr left, NodePtr right);
I8 compareByChar(element a, element b);
I8 compareByFreq(element a, element b);

#endif