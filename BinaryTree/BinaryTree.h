#ifndef BINARY_TREE
#define BINARY_TREE

#include "myTypes.h"

typedef void* element;
typedef I8 (*CompareFunc)(element a, element b);

typedef struct Node {
    element info;
    U8 frequency;
    struct Node* left;
    struct Node* right;
} Node;

typedef Node* NodePtr;

typedef struct {
    NodePtr root;
    CompareFunc comp;
} BinaryTree;

void newTree(BinaryTree* bt, CompareFunc comp);
NodePtr newNode(element e, U8 freq);
void addNode(BinaryTree* bt, element e, U8 freq);
boolean removeNode(BinaryTree* bt, element e);
boolean existNode(BinaryTree* bt, element e);
NodePtr mergeNodes(NodePtr left, NodePtr right);
I8 compareByChar(element a, element b);
I8 compareByFreq(element a, element b);

#endif