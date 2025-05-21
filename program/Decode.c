#include <stdlib.h>
#include "Decode.h"

void decode(FILE *inputFile, NodePtr huffmanTree)
{
    if(!inputFile || !huffmanTree) { return; }

    /* 3. Pega tamanho do arquivo e número de bits válidos no último byte */
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);
    fseek(inputFile, -1, SEEK_END);
    U8 lastByteBits;
    fread(&lastByteBits, sizeof(U8), 1, inputFile);

    long totalBytes = fileSize - 2048 - 1; // tira cabeçalho (2048) e último byte extra

    fseek(inputFile, 2048, SEEK_SET); // vai direto pro início dos dados comprimidos

    /* 4. Decodificação dos bits */
    NodePtr current = huffmanTree;
    FILE *fileOut = fopen("decompressed.txt", "wb");
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