#include <stdlib.h>
#include "BinaryTree.h"

static NodePtr addNodeRec(NodePtr node, CompareFunc comp, element e, U8 freq);
static NodePtr removeNodeRec(NodePtr node, CompareFunc comp, element e);
static NodePtr minValueNode(NodePtr node);

// cria uma nova árvore
void newTree(BinaryTree* bt, CompareFunc comp)
{
    // se não existe árvore para criar, não prossegue
    if(!bt) { return; }

    bt->root = NULL;
    bt->comp = comp;
}

// cria um novo nodo
NodePtr newNode(element e, U8 freq)
{
    // aloca memória para o novo nodo
    NodePtr new = (NodePtr)malloc(sizeof(Node));
    if(!new) { return NULL; } // verifica se a memória do nodo foi alocada com sucesso

    new->info = e;
    new->frequency = freq;
    new->left = NULL;
    new->right = NULL;

    return new;
}

void addNode(BinaryTree* bt, element e, U8 freq)
{
    // se já existe o nodo, não o insere
    // se há árvore para inserir, não tem como inseri-lo
    if(existNode(bt, e) || !bt) { return; }
    bt->root = addNodeRec(bt->root, bt->comp, e, freq);
}

static NodePtr addNodeRec(NodePtr node, CompareFunc comp, element e, U8 freq)
{
    // se chegou ao próximo de uma folha, pode criar nodo
    if(node == NULL) { return newNode(e, freq); }
    I8 compEval = comp(e, node->info);

    // elemento a ser inserido está a direita da árvore
    if(compEval > 0) { node->right = addNodeRec(node->right, comp, e, freq); }

    // elemento a ser inserido está a esquerda da árvore
    else if(compEval < 0) { node->left = addNodeRec(node->left, comp, e, freq); }

    return node; // retorna o nodo para que não se perca a conexão com os filhos
}

boolean removeNode(BinaryTree* bt, element e)
{
    // se não há árvore, não tem elementos para remover
    // se na árvore não existir o nodo, não tem como removê-lo
    if(!bt || bt->root == NULL || !existNode(bt, e)) { return false; }
    bt->root = removeNodeRec(bt->root, bt->comp, e);
    return true;
}

static NodePtr removeNodeRec(NodePtr node, CompareFunc comp, element e)
{
    I8 compEval = comp(e, node->info);

    // elemento a ser removido está a esquerda da árvore
    if(compEval < 0) { node->left = removeNodeRec(node->left, comp, e); }

    // elemento a ser removido está a direita da árvore
    else if(compEval > 0) { node->right = removeNodeRec(node->right, comp, e); }
    
    // encontrado
    else
    {
        // Nodo sem filhos
        if(node->left == NULL && node->right == NULL)
        {
            free(node->info);
            free(node);
            return NULL;
        }

        // Nodo com 1 filho a direita
        else if(node->left == NULL)
        {
            NodePtr temp = node->right;
            free(node->info);
            free(node);
            return temp; // filho da direita substitui o nodo removido
        }

        // Nodo com 1 filho a esquerda
        else if(node->right == NULL)
        {
            NodePtr temp = node->left;
            free(node->info);
            free(node);
            return temp; // filho da esquerda substitui o nodo removido
        }

        // Nodo com 2 fihos
        else
        {
            NodePtr minNode = minValueNode(node->right);
            node->info = minNode->info;
            node->frequency = minNode->frequency;
            node->right = removeNodeRec(node->right, comp, minNode->info);
        }
    }
    return node; // retorna o nodo para que não se perca a conexão com os filhos
}

// retorna o nodo mais a esquerda de um outro nodo
static NodePtr minValueNode(NodePtr node)
{
    while(node->left != NULL) { node = node->left; }
    return node;
}

boolean existNode(BinaryTree* bt, element e)
{
    // se não há árvore, não tem elemento
    if(!bt) { return false; }

    // procura a partir da raíz da árvore
    NodePtr current = bt->root;
    while (current != NULL)
    {
        I8 compEval = bt->comp(e, current->info);

        // achou
        if(compEval == 0) { return true; }

        // elemento a esquerda da árvore
        else if(compEval < 0) { current = current->left; }

        // elemento a direita da árvore
        else { current = current->right; }
    }
    return false; // não achou
}

// une dois nodos em um novo e retorna esse novo
NodePtr mergeNodes(NodePtr left, NodePtr right)
{
    // aloca memória para o novo nodo e verifica se
    // a memória foi alocada com sucesso
    NodePtr new = (NodePtr)malloc(sizeof(Node));
    if(!new) { return NULL; }

    new->info = NULL;
    new->frequency = left->frequency + right->frequency; // nova frequência
    new->left = left;
    new->right = right;

    return new;
}

// compara chars
I8 compareByChar(element a, element b)
{
    return (*(char*)a - *(char*)b);
}

// compara nodos(frequência)
I8 compareByFreq(element a, element b)
{
    Node* nodeA = (Node*)a;
    Node* nodeB = (Node*)b;
    return (nodeA->frequency - nodeB->frequency);
}