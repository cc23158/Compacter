// return -1; -> error
// return 0;  -> correct

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "myTypes.h"
#include "queue/PriorityQueue.h"
#include "algorithm/Code.h"
#include "algorithm/Huffman.h"
#include "file_io/file_io.h"
#include "program/Encode.h"
#include "program/Decode.h"

int doEncode(char *fileName);
int doDecode(char *fileName);

int main(int argc, char *argv[])
{
    // if there's no "./Compacter.out <encode / decode> <fileName>", return -1
    if(argc != 3) { return -1; }

    // if it's to encode a file
    if(strcmp(argv[1], "encode") == 0) { return doEncode(argv[2]); }

    // if it's to decode a file
    else if(strcmp(argv[1], "decode") == 0) { return doDecode(argv[2]); }

    // if it's any other command, return -1
    else { return -1; }
}

int doEncode(char *fileName)
{
    // open the file for binary reading
    FILE *file = fopen(fileName, "rb");
    if(!file) { return -1; }

    // create the priority queue of the file's characters
    U64 frequencies[256];
    PriorityQueue* pq = read(file, frequencies);
    if(!pq)
    {
        fclose(file);
        return -1;
    }

    // build Huffman tree from the priority queue
    NodePtr huffmanTree = buildHuffman(pq);
    if(!huffmanTree)
    {
        fclose(file);
        return -1;
    }
    destroyQueue(pq); // free queue's memory

    // assimilate codes to each character of the Huffman tree
    Code* tableOfCodes[256] = {NULL};
    Code currentCode;
    if(!newCode(&currentCode))
    {
        freeNode(huffmanTree);
        fclose(file);
        return -1;
    }
    buildCodes(huffmanTree, tableOfCodes, &currentCode);

    // create the output compressed file
    const char *output = "compressed.huff";
    FILE *fileOut = fopen(output, "wb");
    if(!fileOut)
    {
        fclose(file);
        freeNode(huffmanTree);
        for(int i = 0; i < 256; ++i)
        {
            if(tableOfCodes[i])
            {
                freeCode(tableOfCodes[i]);
                free(tableOfCodes[i]);
            }
        }
        return -1;
    }

    // add file's extension to compressed file's header
    // 1. goes to the last occurence of "." and get all string until the end of the file name
    // 2. verify if there's content in ext, if doesn't, there's no extension
    // 3. if does have a extension, get the extension -> Ex: *ext = .txt => ext++(txt)
    const char *ext = strrchr(fileName, '.');
    if(!ext) { ext = ""; }
    else { ext++; }

    // 1. calculate the extension size
    // 2. write the extension size in the output file
    // 3. write the extension "name" in the output file
    U8 extLen = (U8)strlen(ext);
    fwrite(&extLen, sizeof(U8), 1, fileOut);
    fwrite(ext, sizeof(char), extLen, fileOut);

    // write the header of each characters frequencies
    // encode the file's content
    fileWrite(fileOut, frequencies);
    encode(file, fileOut, huffmanTree, tableOfCodes);

    // close the files and free memory
    fclose(file);
    fclose(fileOut);
    freeNode(huffmanTree);
    for(int i = 0; i < 256; ++i)
    {
        if(tableOfCodes[i])
        {
            freeCode(tableOfCodes[i]);
            free(tableOfCodes[i]);
        }
    }
    return 0;
}

int doDecode(char *fileName)
{
    // open the file for binary reading
    FILE *file = fopen(fileName, "rb");
    if(!file) { return -1; }

    // read the compressed file's extension
    U8 extLen;
    fread(&extLen, sizeof(U8), 1, file);

    char ext[256] = {0};
    fread(ext, sizeof(char), extLen, file);

    // build the output file name with extension
    char outFileName[300];
    snprintf(outFileName, sizeof(outFileName), "decompressed.%s", ext);

    // read the frequencies of each character in the header
    U64 frequencies[256];
    fread(frequencies, sizeof(U64), 256, file);

    // build the priority queue from the frequencies
    PriorityQueue *pq = createQueueFromFrequencies(frequencies);
    if(!pq)
    {
        fclose(file);
        return -1;
    }

    // build the Huffman tree from the queue
    NodePtr huffmanTree = buildHuffman(pq);
    destroyQueue(pq); // free the queue's memory
    if(!huffmanTree)
    {
        fclose(file);
        return -1;
    }

    // decode the content of input file
    decode(file, huffmanTree, outFileName);

    // close the files and free memory
    fclose(file);
    freeNode(huffmanTree);
    return 0;
}
