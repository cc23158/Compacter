#include <stdlib.h>
#include <stdio.h>
#include "Decode.h"

void decode(FILE *inputFile, NodePtr huffmanTree, char* outFileName)
{
    if(!inputFile || !huffmanTree || !outFileName) { return; }

    // Tamanho total do arquivo
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);

    // Lê bits úteis no último byte
    fseek(inputFile, -1, SEEK_END);
    U8 lastByteBits;
    fread(&lastByteBits, sizeof(U8), 1, inputFile);

    // Calcula posição de início dos dados
    fseek(inputFile, 0, SEEK_SET);
    U8 extLen;
    fread(&extLen, sizeof(U8), 1, inputFile);
    fseek(inputFile, extLen, SEEK_CUR); // pular extensão
    fseek(inputFile, 256 * sizeof(U64), SEEK_CUR); // pular cabeçalho

    long dataStart = ftell(inputFile);
    long totalBytes = fileSize - dataStart - 1; // -1 para byte extra

    NodePtr current = huffmanTree;
    FILE *fileOut = fopen(outFileName, "wb");
    if (!fileOut)
    {
        fclose(inputFile);
        freeNode(huffmanTree);
        return;
    }

    for (long i = 0; i < totalBytes; ++i)
    {
        U8 byte;
        fread(&byte, sizeof(U8), 1, inputFile);

        int bitsToRead = (i == totalBytes - 1) ? lastByteBits : 8;

        for (int b = 7; b >= 8 - bitsToRead; --b)
        {
            U8 bit = (byte >> b) & 1;
            current = bit ? current->right : current->left;

            if (current->left == NULL && current->right == NULL)
            {
                fwrite(&current->character, sizeof(U8), 1, fileOut);
                current = huffmanTree;
            }
        }
    }

    fclose(fileOut);
}
