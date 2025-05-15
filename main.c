#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "myTypes.h"
#include "PriorityQueue/PriorityQueue.h"
#include "BinaryTree/BinaryTree.h"

void read(const char *fileName, PriorityQueue* pq);

int main()
{
    PriorityQueue* pq = createQueue(256);

    if(!pq) { return 1; }

    read("teste.txt", pq);
    
    // Teste: imprime os bytes e suas frequências
    while (!isEmpty(pq)) {
        NodePtr node = dequeue(pq);
        printf("Byte: 0x%02X (%c), Freq: %lu\n", *(unsigned char *)node->info, *(char *)node->info, node->frequency);
        free(node->info);
        free(node);
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
    U64 frequencies[256] = {0};

    unsigned char byte;
    // fseek(f, deslocamento, ptReferencia)
    // SEEK_SET, SEEK_END, SEEK_CUR
    // fread -> retorna um inteiro com a quantidade de bytes lidos
    while (fread(&byte, sizeof(unsigned char), 1, file) == 1) { frequencies[byte]++; }

    fclose(file);

    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            // Cria cópia do byte como ponteiro (element)
            unsigned char *ptr = malloc(sizeof(unsigned char));
            *ptr = (unsigned char)i;

            NodePtr node = newNode(ptr, frequencies[i]);
            enqueue(pq, node);
        }
    }
}

int countChar(char *str, char c)
{
  int length = strlen(str);
  int count = 0;
  
  for (int i = 0; i < length; i++) { if (str[i] == c) count++; }

  return count;
}