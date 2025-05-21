#include <stdlib.h>
#include "Huffman.h"

// create a priority queue from frequencies array
PriorityQueue* createQueueFromFrequencies(U64 frequencies[256])
{
    PriorityQueue* pq = createQueue(256);
    if(!pq) { return NULL; }
    
    for(int i = 0; i < 256; i++)
    {
        if(frequencies[i] > 0)
        {
            U8 *ptr = malloc(sizeof(U8));
            *ptr = (U8)i;
            
            enqueue(pq, newNode(*ptr, frequencies[i]));
        }
    }
    return pq;
}

// build Huffman tree
NodePtr buildHuffman(PriorityQueue* pq)
{
    // if there's no queue or its empty, return null
    if(!pq || isEmpty(pq)) { return NULL; }
    
    // only 1 node
    if(pq->size == 1)
    {
        NodePtr root = dequeue(pq);
        NodePtr newNodeTree = newNode('\0', root->frequency);
        if(!newNodeTree)
        {
            free(root);
            return NULL;
        }
        
        newNodeTree->left = root;
        enqueue(pq, newNodeTree);
    }
    
    // > 1 node
    else
    {
        while(pq->size > 1)
        {
            NodePtr node1 = dequeue(pq);
            NodePtr node2 = dequeue(pq);
            if(!node1 || !node2)
            {
                if(node1) { free(node1); }
                if(node2) { free(node2); }
                return NULL;
            }
            
            // new node with null character and sum of node1 and node2 frequencies
            NodePtr newNodeTree = newNode('\0', node1->frequency + node2->frequency);
            if(!newNodeTree)
            {
                free(node1);
                free(node2);
                return NULL;
            }
            
            newNodeTree->left = node1;
            newNodeTree->right = node2;
    
            enqueue(pq, newNodeTree);
        }
    }
    return dequeue(pq); // return the Huffman tree
}

// build the characters codes
void buildCodes(NodePtr node, Code* table[256], Code* current)
{
    // if the node value is null, stop the cycle
    if(!node) { return; }

    // if is a leaf, link the code with the character
    if(node->left == NULL && node->right == NULL)
    {
        Code* code = (Code*)malloc(sizeof(Code));
        if(!code) { return; }
        
        if(!clone(*current, code))
        {
            free(code);
            return;
        }
        table[node->character] = code;
        return;
    }
    
    if(!addBit(current, 0)) { return; } // goes to left subtree
    buildCodes(node->left, table, current);
    if(!removeBit(current)) { return; } // remove bit 0 to continue in right subtree

    if(!addBit(current, 1)) { return; } // goes to right subtree
    buildCodes(node->right, table, current);
    if(!removeBit(current)) { return; } // remove bit 1 to continue in left subtree
}