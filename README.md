# File Compression Application

This program is made up of two applications: hencode.c which utilizes huffman encoding to compress a file, and hdecode.c to decompress a file.

Created by Connor Dye as a California Polytechnic University Project.

## Hencode.c (Huffman Encode) Features
-Usage: ./hencode infile [ outfile ] where the infile is the file to be encoded and outfile is an optional argument (output goes to stdout if no outfile is specified)
-encodes input into a binary Huffman-coded file

## Hdecode.c (Huffman Decode) Features
-Usage: ./hdecode [ ( infile | - ) [ outfile ] ] where infile and outfile are optional file arguments (input and output come from stdin and stdout otherwise)
-decodes input and restore the original data.

## Notes

