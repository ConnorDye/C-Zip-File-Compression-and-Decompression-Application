#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bit.h"
#include "list.h"
/*UNIX HEADER FILES */
#include <unistd.h> /*constants for UNIX IO */
#include <fcntl.h> /*holds file function */
#include <sys/stat.h>
#include <arpa/inet.h>

/*CONSTANTS */
#define BUFFSIZE   5000 /*max size header can be is 1281 bytes */


void printArr(Node** nodeArr, int arrSize);
int contains(Node** nodeArr, int ch, int arrSize);
int compare(const void *c1, const void *c2);
List *arrToList(Node* arr[], int arrSize);
void printList(List *lst);
void listToTree(List *list);
void printhuff(Node* nodeArr[], int arrSize);
int compareToPrint(const void *c1, const void *c2);
void generatehashtable(Node *nodeArr[], Node *hashTable[], int arrSize);
void testhashtable(Node *hashTable[], int arrSize);
int readHeader(Node* nodeArr[], int arrSize, int fileDescriptor,
                                                            bits *bitstream);
void decodeFile(List *tree, bits *bitstream, int fileDescriptor,
                                            int fileDescOut, int totalChars);

/*THIS PROGRAM WILL CREATE A HUFFMAN TREE FORM A FILE OF CHARACTERS
AND ENCODE THE FILE TO COMPRESS IT FOR SENDING OVER A NETWORK */

int main(int argc, char* argv[]){
    /*UNIX IO DECLARATIONS */
    struct stat sb; /*for empty file check */
    int fileDescriptor; /*used by linux IO to identify file */
    int fileDescOut; /*for outfile */
    char buff[100]; /*buffer size associated with read()*/
    char* fileName = NULL;
 
    /*FILE PROCESSING*/
    int option = 0; /*defined for getopt */
    
    /*ASSOCIATED WITH DATA STRUCTURES */
    int arrSize = 256; /*max of 256 character */
    Node **nodeArr = calloc(arrSize, sizeof(Node *));
    List *list = NULL; /*linked list of nodes */
    int arr[256]; /*only for tree.c printTree to work */
    bits *bitstream = createbitstream(BUFFSIZE);
    int totalChars = 0; /*total characters in file */

    if((option = getopt(argc, argv, "")) != -1){ /*program should take no
                                                                    commands*/
        fprintf(stderr, "USAGE: ./hdecode [(infile | -) (outfile)]\n");
        exit(EXIT_FAILURE);
    }
    
    if(optind == argc){
        fprintf(stderr, "USAGE: ./hdecode [(infile | -) (outfile)]\n");
        exit(EXIT_FAILURE);
    }

    if(optind < argc){
        fileName = argv[optind];
        optind++; /*increment CL arg */
        if(fileName[0] == '-'){
            fileDescriptor = STDIN_FILENO;
        }
        else if((fileDescriptor = open(fileName, O_RDONLY)) == -1){
            perror("ISSUE WITH FILE: ");
            fprintf(stderr, "FILE WITH ISSUE IS: %s\n", fileName);
            exit(EXIT_FAILURE);
        }
        
        
    }

    /*if outfile given, open it */
    if(optind < argc) /*for out file. No permission bits needed for existing*/
    {
        if((fileDescOut = open(argv[optind], O_WRONLY| O_TRUNC | O_CREAT,
        S_IRUSR | S_IRGRP | S_IWUSR | S_IWGRP)) == -1){
            perror(argv[optind]);
            exit(EXIT_FAILURE);
        }
        optind++;
    }
    else{
        fileDescOut = STDOUT_FILENO;
    }
    
    /*ALGORITHM AS FOLLOWS:
        *CREATE ARRAY FROM HEADER
        *QUICKSORT BASED ON FREQUENCY AND CHARACTER SECONDARY
        *TRANSFORM INTO A LIST
        *TRANSFORM INTO HUFFMAN TREE
        *GENERATE CHARACTERS FROM THIS TREE
    */


    /*empty file check */
    if(fileDescriptor != -1 && fileDescriptor != STDIN_FILENO){ 
        if (fstat(fileDescriptor, &sb) == -1) {
           perror("lstat");
           exit(EXIT_FAILURE);
        }

        if(sb.st_size == 0){
            if(write(fileDescOut, buff, 0) != 0){                               
                perror("EMPTY FILE ERR");
            }
            exit(EXIT_SUCCESS);
        }

    }
    
    totalChars = readHeader(nodeArr, arrSize, fileDescriptor, bitstream);
    qsort(nodeArr, arrSize, sizeof(Node *), compare);
    list = arrToList(nodeArr, arrSize);
    /*printArr(nodeArr, arrSize);
    printList(list); */
    listToTree(list);
    generatecodes(list->head, arr, 0); /*generates huffman codes into nodes */
    /*read data*/

    qsort(nodeArr, arrSize, sizeof(Node *), compareToPrint);/*sort alphabet*/
    /*printhuff(nodeArr, arrSize);*/
    decodeFile(list, bitstream, fileDescriptor, fileDescOut, totalChars);

    return 0;
}

/*helper function to check if array contains node, and increments frequency
if so */
int contains(Node** nodeArr, int ch, int arrSize){
    int j;
    for(j = 0; j < arrSize; j++){
        if(nodeArr[j]->data == ch){
            (nodeArr[j]->frequency)++;
            return 1;
        }
    }

    return 0; /*return 0 if arrNode does not already contain the character */
}      


void printArr(Node** nodeArr, int arrSize){
    int j;
    for(j=0; j < arrSize; j++){
        if(nodeArr[j] == NULL)
            return;
        printf("Node at index %d is word %c with frequency %d\n", j, 
                                    nodeArr[j]->data, nodeArr[j]->frequency);
    }

}

void printList(List *lst){
    Node* temp = lst->head;
    int j = 0;
    while(lst != NULL){
        if(temp == NULL)
            return;
        printf("Node at index %d is word %c with frequency %d\n", j,            
                                     temp->data, temp->frequency);
        j++;
        temp = temp->next;
    }


}


/*compare return less than 0 is c1 goes before c2, greater than
0 if p1 goes after second */
    /*this comparator is for the initial compare to build tree
    *we will need another comparator to print in order based on character */
int compare(const void *c1, const void *c2){
    Node *a = *(Node **)c1;
    Node *b = *(Node **)c2;
    if(!a && !b)  /*if null*/
        return 0;
    if(!a)
        return 1;
    if(!b)
        return -1;
    
    
    if(a->frequency == b->frequency)
        return a->data - b->data;

    /*if(a->frequency != b->frequency)*/
    return a->frequency - b->frequency;
}

/*comparator to print in alphabetical order */
int compareToPrint(const void *c1, const void *c2){
    Node *a = *(Node **)c1;
    Node *b = *(Node **)c2;
    if(!a && !b)  /*if null*/
        return 0;
    if(!a)
        return 1;
    if(!b)
        return -1;
    
    return a->data - b->data;
}


List *arrToList(Node* arr[], int arrSize){
    List *nodeList = lstcreate(); 
    int i;
    for(i = 0; (i < arrSize) && (arr[i] != NULL); i++){
       Node* node = arr[i];
       /*nodeList->head = insert(nodeList->head, i, node);*/
       if((lstadd(nodeList, i, node)) == 1)
            fprintf(stderr, "ERROR ADDING TO LIST");
    }
    
    return nodeList;
    
}

void listToTree(List *list){
    while(list->size > 1){
        int i; /*defined for for loop */
        int flag = 0; /*boolean to see if we added val */

        /*get lchild and rchild */ 
        Node *tempA = lstget(list, 0); 
        Node *tempB = lstget(list, 1);
        
        /*create tree node */
        Node *treeNode = createNode(0); /*let 0 represent null char */
        treeNode->frequency = tempA->frequency + tempB->frequency;
        treeNode->left = tempA;
        treeNode->right = tempB;
        
        /*remove from list */
        lstremove(list, 0);
        lstremove(list, 0);

        /*insert at an index */
        for(i = 0; i <= list->size && flag == 0; i++){
            if(i == list->size){ /*if adding at tail */
                if((lstadd(list, i, treeNode)) == 1){ /*returns 1 on failure*/ 
                    fprintf(stderr, "ERROR ADDING TREE NODE TO LIST");
                }
                else{
                    flag = 1;
                }
            }
            else if(treeNode->frequency < (lstget(list,i))->frequency){
                if((lstadd(list, i, treeNode)) == 1){ 
                     fprintf(stderr, "ERROR ADDING TREE NODE TO LIST");
                }
                else{
                    flag = 1;
                }
            }
            /*if frequencies are equal check character */
            else if (treeNode->frequency == (lstget(list,i))->frequency){
                if((lstadd(list, i , treeNode)) == 1){ 
                    fprintf(stderr, "ERROR ADDING TREE NODE TO LIST");
                }
                else{
                    flag = 1;
                }
            }
        }

    }
}

void printhuff(Node* nodeArr[], int arrSize){
    int j;
    for(j=0; j < arrSize; j++){
        if(nodeArr[j] == NULL)
            return;
        if(nodeArr[j]->data <= 15){
            printf("0x0%x: %s", nodeArr[j]->data, nodeArr[j]->code);
        }
        else{
            printf("0x%x: %s", nodeArr[j]->data, nodeArr[j]->code);
        }
        putchar('\n');
    }
}

/*FUNCTIONALITY
    *THIS FUNCTION ONLY WORKS IF BUFFERSIZE IS ATLEAST EQUAL TO THE MAX HEADER
    SIZE: 256 bytes + 256 * 4 + 1
    *getbyte needs to check if
    *this function will only read the exact header size, that way
    decodeFile can handle the rest
    *RETURNS TOTAL NUMBER OF CHARACTERS IN FILE
*/

int readHeader(Node* nodeArr[], int arrSize, int fileDescriptor,
                                                            bits *bitstream){
    int bytesRead; 
    int numChar; /*number of characters in nodeArr */
    int bytesInHeader;
    int ch; /*character of node */
    uint8_t byteArr[4]; /*for integer conversion*/
    int count;
    int index = 0, i = 0; /*index to add node at */
    Node* temp;
    int totalChars = 0;

    /*COMPUTE HEADER SIZE*/
    bytesRead = read(fileDescriptor, bitstream->buff, 1);
    if(bytesRead < 0)
        perror("read error");
    if(bytesRead == 0)
        fprintf(stderr, 
                "Empty file in read header, should've been handled earlier");
    numChar = getbyte(bitstream);
    bytesInHeader = (numChar + 1) * 5;
    bitstream->nextByte = 0; /*reset byte to beginning when we overwrite*/
    resetBits(bitstream);

    if((bytesRead = read(fileDescriptor, bitstream->buff, bytesInHeader)) 
                                                            != bytesInHeader){
            
        fprintf(stderr, "Error reading header");
    }
    else{
        /*numChar = getbyte(bitstream);
          bytesInHeader = (numChar + 1) * 5; */
        while(bitstream->nextByte < bytesInHeader){
            ch = getbyte(bitstream);
            count = 0;
            /*get 4 bytes of frequency */
            for(i = 0; i < 4; i ++){
                byteArr[i] = getbyte(bitstream);
            }
            count = setinteger(byteArr);
            totalChars = totalChars + count; /*holds total number of chars */
            temp = createNode(ch);
            temp->frequency = count;
            nodeArr[index] = temp;
            index++;
            if(bitstream->nextByte == bitstream->size)
                fprintf(stderr, "FAILED TO READ DECODE HEADER\n");
        }
        

    }
    
    if(bytesRead < 0){
        perror("read error");
    }

    bitstream->nextByte = 0; /*RESET BUFFER TO PROCESS FILE CONTENTS*/
    bitstream->bytesRead = 0;
    resetBits(bitstream);
    return totalChars;
}

/*FUNCTIONALITY:
    *WILL DECODE AND WRITE FILE
    *BUFFERSIZE > HEADER, so decodeFile should start */

void decodeFile(List *tree, bits *bitstream, int fileDescriptor,
                                            int fileDescOut, int totalChars){
    int size;
    int bit;
    Node* root = tree->head;
    char writeBuff[BUFFSIZE];
    int index = 0; /*index to add character*/
    int charRead = 0; 
    resetBits(bitstream);
    if((size = read(fileDescriptor, bitstream->buff, BUFFSIZE)) > 0){
        bitstream->bytesRead  = 0; /*bytesProcessed*/
        /*tree with more than one character*/
        while (root->right != NULL && root->left != NULL 
                                                && charRead < totalChars){
            
            if((bit = getbitstream(bitstream)) == -1){ /*if we read buffer */
                resetBits(bitstream);
                size = read(fileDescriptor, bitstream->buff, BUFFSIZE);
                if(size == 0){ /*if no more file, write and return */
                     if(write(fileDescOut, writeBuff, index) != index)
                         perror("write error");
                     return;
                }
                if(size < 0)
                    perror("read error");
                
                bitstream->nextByte = 0; /*start at beginning of buffer*/
                continue; /*continue to next iteration to get bit if -1*/
            }

            /*traverse tree */
            if(bit == 1)
                root = root->right;
            if(bit == 0)
                root = root->left;
            
            /*once leaf reached, write data to our buffer to be written */
            if(root->right == NULL && root->left == NULL){
                writeBuff[0] = root->data;
                write(fileDescOut, writeBuff, 1);
                root = tree->head; /*reset root to tree head*/
                charRead++;
            }
            
        }
    }
    
    /*one character tree*/
    if(root->right == NULL && root->left == NULL){
        int i;
        for(i = 0; i < root->frequency; i++){
            writeBuff[index] = root->data;
            index++;
            
            if(index == BUFFSIZE){
                /*write index (aka number of chars added */
                if(write(fileDescOut, writeBuff, index) != index)
                    perror("write error");
                index = 0; /*loop to beginning of buffer */

            }
        }
        /*root->frequency != BUFFSIZE prevents double writes for that case*/
        if(write(fileDescOut, writeBuff, index) != index 
                                                && root->frequency != BUFFSIZE)
            perror("write error");
    }

    
    if(size < 0)
        perror("read error");

}

/*FUNCTIONALITY
    *GENERATES 256 character array (all ascii values) which can
    be accessed by a character (int) hash (i.e a at index 97, b at 98, etc) 
    *hashtable uses calloc to initialize to null, so if you access
    a character that you know exist and there is a segfault check here */
void generatehashtable(Node *nodeArr[], Node *hashTable[], int arrSize){
    int i;
    unsigned int hash;
    for(i = 0; i < arrSize && nodeArr[i] != NULL; i++){
        hash = nodeArr[i]->data;
        hashTable[hash] = nodeArr[i];
    }
}

void testhashtable(Node *hashTable[], int arrSize){
    int i;
    for(i = 0; i < arrSize; i++){
        if(hashTable[i] == NULL)
            continue;    
        printf("%c character exists at index %d\n", hashTable[i]->data, i);
    }
}

