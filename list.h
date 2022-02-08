/*
 * Defines structures and prototypes for linked lists.
 * NOTE: Do not alter this file.
 */

/* Node: Represents a single element in a linked list. */
#ifndef LISTH
#define LISTH
typedef struct Node {
   int data; /*holds a char */
   unsigned int frequency; /*number of times a word occurs */
   char code[256];
   struct Node *next; /*linked list implementation */
   struct Node* left; /*binary tree implementation */
   struct Node* right;
} Node;

/* List: Represents a linked list. */
typedef struct List {
   Node *head;
   unsigned int size;
} List;


/*FOR LINKED LIST*/
unsigned int lstsize(List *);
Node* lstget(List *, unsigned int);
int lstset(List *, unsigned int, int data);
int lstadd(List *, unsigned int, Node* newNode); /*accepts the allocated node*/
int lstremove(List *, unsigned int);
void lstdestroy(List *);
List *lstcreate();
Node* insert(Node *head, int idx, Node* nn);

/*FOR BINARY TREE */
Node* createNode(int data);                                                     
void insertLeft(Node* root, Node* insert);                                      
void insertRight(Node* root, Node* insert);                                     
void inorder(Node* root);                                                       
void generatecodes(Node* root, int arr[], int top);

#endif
