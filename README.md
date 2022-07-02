# File Compression Application

This program consists of two applications: hencode.c which utilizes huffman encoding to compress a file, and hdecode.c to decompress a file.

Created by Connor Dye as a California Polytechnic University Project.

## Hencode.c (Huffman Encode) Features
- Usage: ./hencode infile [ outfile ] where the infile is the file to be encoded and outfile is an optional argument (output goes to stdout if no outfile is specified)
- encodes input into a binary huffman encoded file

## Hdecode.c (Huffman Decode) Features
- Usage: ./hdecode [ ( infile | - ) [ outfile ] ] where infile and outfile are optional file arguments (input and output come from stdin and stdout if these are not specified)
- decodes input and restores the original data.

## Notes
- list.c is a custom built and error tested linked list data structure library that works on the same node struct as tree.c
- tree.c is a custom built binary tree data structure library that works on the same node struct as list.c
- together these allow for the huffman algorithm to be carried out:  1.) generate a histogram of all the characters in the file  2.) generate a linked list of the characters in ascending order of frequency  3.) the first two nodes of the list become a tree with the root being the sum of the two removed nodes and the left and right child being the removed nodes  4.) reinsert tree from the previous step at the head of the list and repeat until your list consists of only a single node  5.) trace the tree to get the huffman codes to encode
- bit.c is our bit library that allows us to compress based on the huffman codes; this library allows for an abstraction of the handling of bits to compress and decompress
- helper functions to create headers to recreate the tree are in hdecode.c and hencode.c
- hencode.c and hdecode.c includes the parsing and main functionality to put these libraries together

