#include <stdlib.h>
#include <stdio.h>
#include "Decode.h"

void decode(FILE *inputFile, NodePtr huffmanTree, char* outFileName)
{
    if(!inputFile || !huffmanTree || !outFileName) { return; }

    // file's total size
    fseek(inputFile, 0, SEEK_END);
    long fileSize = ftell(inputFile);

    // read how many bits to consider in the last byte
    fseek(inputFile, -1, SEEK_END);
    U8 lastByteBits;
    fread(&lastByteBits, sizeof(U8), 1, inputFile);

    // calculate the position where the compressed data starts
    fseek(inputFile, 0, SEEK_SET);
    U8 extLen;
    fread(&extLen, sizeof(U8), 1, inputFile);
    fseek(inputFile, extLen, SEEK_CUR);            // skip extension
    fseek(inputFile, 256 * sizeof(U64), SEEK_CUR); // skip header

    // start of compressed content found
    long dataStart = ftell(inputFile);
    long totalBytes = fileSize - dataStart - 1; // -1 for extra byte

    // open the output file for the decompressed content
    NodePtr current = huffmanTree;
    FILE *fileOut = fopen(outFileName, "wb");
    if(!fileOut)
    {
        fclose(inputFile);
        freeNode(huffmanTree);
        return;
    }

    // read all bytes from the file
    for(long i = 0; i < totalBytes; ++i)
    {
        // read current byte
        U8 byte;
        fread(&byte, sizeof(U8), 1, inputFile);

        // if it's not the last byte, read 8 bits = 1 byte
        // if it is the last byte, read ONLY the valid bits
        int bitsToRead = (i == totalBytes - 1) ? lastByteBits : 8;

        // read each bit from current byte, read from left to right
        for(int b = 7; b >= 8 - bitsToRead; --b)
        {
            // move all bits to right and get the last bit (Ex: 10101010 -> 00000001 => 1)
            U8 bit = (byte >> b) & 1;

            // tree navigation, 0 -> left / 1 -> right
            current = bit ? current->right : current->left;

            // if a leaf is reached, a character is defined
            if(current->left == NULL && current->right == NULL)
            {
                // write the character
                fwrite(&current->character, sizeof(U8), 1, fileOut);
                current = huffmanTree;
            }
        }
    }
    fclose(fileOut); // close file
}