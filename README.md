# C-Zip: File Compression and Decompression Application

This program consists of two applications: `hencode.c` which utilizes huffman encoding to compress a file, and `hdecode.c` to decompress a file.

Created by Connor Dye as a California Polytechnic University Project.

## Hencode.c (Huffman Encode) Features
- `Usage: ./hencode infile [ outfile ]` where the infile is the file to be encoded and outfile is an optional argument (output goes to stdout if no outfile is specified)
- encodes input into a binary huffman encoded file

## Hdecode.c (Huffman Decode) Features
- `Usage: ./hdecode [ ( infile | - ) [ outfile ] ]` where infile and outfile are optional file arguments (input and output come from stdin and stdout if these are not specified)
- decodes input and restores the original data.

## Notes
- `list.c` is a custom built and error tested linked list data structure library that works on the same node struct as tree.c
- `tree.c` is a custom built binary tree data structure library that works on the same node struct as list.c
- together these allow for the huffman algorithm to be carried out:  **`1.)`** generate a histogram of all the characters in the file  **`2.)`** generate a linked list of the characters in ascending order of frequency  **`3.)`** the first two nodes of the list become a tree with the root being the sum of the two removed nodes and the left and right child being the removed nodes  **`4.)`** reinsert tree from the previous step at the head of the list and repeat until your list consists of only a single node  **`5.)`** trace the tree to get the huffman codes to encode
- `bit.c` is our bit library that allows us to compress based on the huffman codes; this library allows for an abstraction of the handling of bits to compress and decompress
- helper functions to create headers to recreate the tree are included in hdecode.c and hencode.c
- hencode.c and hdecode.c includes the parsing and main functionality to put all libraries together
- How the Huffman algorithm works: **`1.)`** A variable length encoding is created based on the frequency that a character occurs in the file (e.g you want more frequent characters to be encoded with a smaller number of bits **`2.)`** To do this, we first find the counts of all characters in the file  **`3.)`**  We then place all the characters in a linked list ordered by frequency **`4.)`** From here, we generate the tree by removing the first two nodes, and placing the characters at the leaves and creating a parent node which holds the combined frequency of the leaves **`5.)`** We reinsert the node at the front of our linked list tree stucture  **`6.)`** We repeat this until all of our characters are at the leaves **`7.)`** We can now find our compressed encodings by traversing the tree; when we move right our bit is 1, and when we move left our bit is 0
- ALGORITHM IN CODE AS FOLLOWS:
        * PROCESS FILE INTO AN ARRAY OF NODES
        * QUICKSORT BASED ON FREQUENCY AND CHARACTER SECONDARY
        * TRANSFORM INTO A LIST
        * TRANSFORM INTO HUFFMAN TREE
        * GENERATE HUFFMAN CODES
        * 
