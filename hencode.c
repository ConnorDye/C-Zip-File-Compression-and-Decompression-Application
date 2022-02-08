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
#include <assert.h>

/*CONSTANTS */
#define BUFFSIZE   5000


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
void writeFile(Node* nodeArr[], Node* hashTable[], int numChar, 
                            int arrSize, int fileDescriptor, int fileDescOut);


/*THIS PROGRAM WILL CREATE A HUFFMAN TREE FORM A FILE OF CHARACTERS
AND ENCODE THE FILE TO COMPRESS IT FOR SENDING OVER A NETWORK */

int main(int argc, char* argv[]){
    /*UNIX IO DECLARATIONS */
    struct stat sb; /*for empty file check */
    int fileDescriptor; /*used by linux IO to identify file */
    int fileDescOut; /*for outfile */
    unsigned int buff[100] = {0}; /*buffer size associated with read()*/
    char* fileName = NULL; /*file to be concatenated with path */
 
    /*FILE PROCESSING*/
    int option = 0; /*defined for getopt */
    unsigned int ch = 0;
    
    /*ASSOCIATED WITH DATA STRUCTURES */
    int arrSize = 256; /*max of 256 character */
    Node **nodeArr = calloc(arrSize, sizeof(Node *));
    Node **hashTable = calloc(arrSize, sizeof(Node *));
    List *list = NULL; /*linked list of nodes */
    int arr[256] = {0}; /*only for tree.c printTree to work */
    int numItems = 0; /*number of items in the array */

    if((option = getopt(argc, argv, "")) != -1){ /*program should take no
                                                                    commands*/
        fprintf(stderr, "USAGE: ./lab03 [file name]\n");
        exit(EXIT_FAILURE);
    }
    
    if(optind == argc){
        fprintf(stderr, "USAGE: ./lab03 [file name]\n");                        
        exit(EXIT_FAILURE);
    }

    if(optind < argc){
        int i = 0; /*index to add node and holds number of nodes in array */ 
        long int size = 0; /*size of bytes read */
        fileName = argv[optind];
        optind++; /*increment CL arg */
        if((fileDescriptor = open(fileName, O_RDONLY)) == -1){
             perror("ISSUE WITH FILE: ");             
             fprintf(stderr, "FILE WITH ISSUE IS: %s\n", fileName);
             exit(EXIT_FAILURE); 
        }
        
        if((fileDescriptor = open(fileName, O_RDONLY)) == -1){
            perror("ISSUE WITH FILE: ");
            fprintf(stderr, "FILE WITH ISSUE IS: %s\n", fileName);
            exit(EXIT_FAILURE);
        }

        
        /*TO PASSS BINARY TEST CASE, YOUR CHARS MUST BE UNSIGNED*/
        while((size = read(fileDescriptor, buff, 1)) > 0){ /*0 is EOF*/
          int j;
          for(j = 0; j < size; j++){  
            ch = buff[j]; /*character read into buffer*/

            if(i > arrSize){
                fprintf(stderr, "ERROR: there is only 256 possible characters"
                                 " and you are accessing memory outside array"
                                 " bound\n");
                exit(EXIT_FAILURE);
            }
            
            if(contains(nodeArr, ch, i)){
                /*if arrNodes contains char, count if incremented*/
            }   
            else{
                Node *nn = createNode(ch);
                nodeArr[i] = nn;
                i++; /*if we add a node increment index to add next node*/
            }
          }
          numItems = i - 1; /*number items in the array */
        }

        if(size == -1){ /*read returns -1 on read error */
            perror("ERROR READING DATA: ");
            exit(EXIT_FAILURE);
        }

        if(fileDescriptor != -1){ /*instead of close lseek back */
           /* close(fileDescriptor); */
           lseek(fileDescriptor, 0, SEEK_SET);
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
        *PROCESS FILE INTO AN ARRAY OF NODES
        *QUICKSORT BASED ON FREQUENCY AND CHARACTER SECONDARY
        *TRANSFORM INTO A LIST
        *TRANSFORM INTO HUFFMAN TREE
        *GENERATE HUFFMAN CODES
    */

    
    if(fileDescriptor != -1){ /*exit on empty file */
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

    qsort(nodeArr, arrSize, sizeof(Node *), compare);
    list = arrToList(nodeArr, arrSize);
    /*printArr(nodeArr, arrSize);
    printList(list);*/ 
    listToTree(list);
    generatecodes(list->head, arr, 0); /*generates huffman codes into nodes */
    qsort(nodeArr, arrSize, sizeof(Node *), compareToPrint);/*sort alphabet*/
    /*printhuff(nodeArr, arrSize);*/
    generatehashtable(nodeArr, hashTable, arrSize);
    /*testhashtable(hashTable, arrSize);*/
    writeFile(nodeArr, hashTable, numItems, arrSize, 
                            fileDescriptor, fileDescOut); 


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

void writeFile(Node* nodeArr[], Node* hashTable[], int numChar, int arrSize, 
                                    int fileDescriptor, int fileDescOut){
    /*vars to process codes */
    int size; /*number of bytes read */
    uint8_t buff[BUFFSIZE];
    unsigned int character;
    Node *temp;
    bits *bitstream = createbitstream(BUFFSIZE);
    int bit; /*convert char to int bit from node*/
    /*vars to write header */
    int j, i; /*index for for loops */
    uint8_t *ch = (uint8_t *)malloc(sizeof(uint8_t)); /*for header*/
    int* count = (int *)malloc(sizeof(int));

    /*write numChar*/
    *ch = numChar;
    if(write(fileDescOut, ch, sizeof(uint8_t)) != sizeof(uint8_t))
        perror("write err");
    for(j=0; j <= numChar; j++){
        *ch = nodeArr[j]->data; /*cast to char */
        *count = htonl(nodeArr[j]->frequency); 
        /*write ch to file */
        if(write(fileDescOut, ch , sizeof(uint8_t)) != sizeof(uint8_t))
            perror("write err");
       /*write(node[j]->frequency) to file */ /*cast to network byte*/
        if(write(fileDescOut, count, sizeof(int)) != sizeof(int))
            perror("write err");
    }
    free(ch);
    free(count);
    
    /*process file and use getbit abstraction */
    while((size = read(fileDescriptor, buff, BUFFSIZE)) > 0){
       for(j = 0; j < size; j++){ 
            /*process huffman codes and write to file*/
            character = buff[j];
            temp = hashTable[character]; /*node with code to retrieve */
            for(i = 0; temp->code[i] != '\0'; i++){
               /*if bitstream is full, file is written */
               bit = temp->code[i] - '0';
               if(setbitstream(bitstream, bit)){
                    if(write(fileDescOut, bitstream->buff, bitstream->size) 
                                                        != bitstream->size)
                        perror("write err");
                    bitstream->bytesRead = 0;
                    resetBits(bitstream);
               }
            }
       }
    }

    if(size < 0)
        perror("READ ERROR: ");

    while(bitstream->nextBit != 7){ /*pad with zeroes if leftover bytes*/
        setbitstream(bitstream, 0);
    }
    
    if(write(fileDescOut, bitstream->buff, bitstream->bytesRead) 
                                        != bitstream->bytesRead){
            perror("write err");
    }
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
        assert(hashTable[i]->data == i);
    }

    assert(hashTable[10]->data == '\n');
}

/*
void writeFile(Node* nodeArr[], int numChar, int arrSize){ *pass i - 1 to 
                                                            numChar
    int j;
    int byteSize = 8;
    unsigned char byte = byte & (~1); *byte initialized to all 0
    unsigned int count = count & (~1);
    *write num - 1 (num of uniq chars) to file *
    for(j = 0; j < 8; j++){
        if(numChar & (1 << j)) *if bit is set, set bit in byte
            setbit(j, &byte);
    }
    *write using unix to file
    
    for(j=0; j < arrSize && nodeArr != NULL; j++){
        byte = byte & (~1);
        int i;
        for(i = 0; i < byteSize; i++){ *write byte of character 
            if((nodeArr[j]->data) & (1<<i))
                setbit(i, &byte);
        }
        *write num -1 to file using unix 
        
        *write(node[j]->frequency) to file 
    }

    *read through file and generate code 

} */
