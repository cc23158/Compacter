build:
	gcc main.c queue/PriorityQueue.c Algorithm/Code.c Algorithm/Huffman.c file_io/file_io.c  program/Encode.c program/Decode.c -o Compacter.out

execute:
	./Compacter.out