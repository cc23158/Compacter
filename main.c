#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myTypes.h"
#include "PriorityQueue/PriorityQueue.h"
#include "BinaryTree/BinaryTree.h"

void read(const char *fileName, PriorityQueue* pq);

int main()
{
    // criar fila
    PriorityQueue* pq = createQueue(256);
    
    // verifica se a fila foi criada corretamente
    if(!pq) { return 1; }

    read("teste.txt", pq);
    
    // percorre a fila exibindo os nodos
    while (!isEmpty(pq)) {
        NodePtr node1 = dequeue(pq);
        NodePtr node2 = dequeue(pq);
        
        NodePtr nodeTree = newNode("", node1->frequency + node2->frequency);
    }

    destroyQueue(pq);
    return 0;
}

void read(const char *fileName, PriorityQueue* pq)
{
    FILE *file = fopen(fileName, "rb+"); // leitura em modo binário

    // se o arquivo não foi aberto com sucesso, não pode ler
    if (file == NULL) { exit(1); }

    // vetor de U8
    U8 frequencies[256] = {0};

    U8 byte;
    // fseek(f, deslocamento, ptReferencia)
    // SEEK_SET, SEEK_END, SEEK_CUR
    // fread -> retorna um inteiro com a quantidade de bytes lidos
    
    // faz a leitura de 1 byte / char do arquivo file,
    // armazena o char lido no endereço da variável byte
    // e aumenta a frequência desse char no array
    while (fread(&byte, sizeof(U8), 1, file) == 1) { frequencies[byte]++; }

    // fechar arquivo de leitura
    fclose(file);

    for (int i = 0; i < 256; i++) {
        // verifica se o caracter apareceu no arquivo lido
        if (frequencies[i] > 0) {
            U8 *ptr = malloc(sizeof(U8));
            *ptr = (U8)i; // ponteiro para o char

            // cria nodo e adiciona na fila
            NodePtr node = newNode(ptr, frequencies[i]);
            enqueue(pq, node);
        }
    }
}