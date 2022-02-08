#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

/*typedef struct Node {
    int data; *will hold a char
    int frequency; *how many times the character occurs
    struct Node *left, *right;

}Node;*/


Node* createNode(int data){
    Node* temp = (Node*)malloc(sizeof(Node));
    /*int* tempData = (int*)malloc(sizeof(int)); 
    *tempData = data; */
    temp->data = data;
    temp->left = NULL;
    temp->right = NULL;
    temp->frequency = 1; /*when created frequency will be at 1 */
    return temp;
}

/*insertLeft & insertRight work as follows
    *Description: in Huffman coding when we use a binary tree, 
    we will always be inserting at either the left child or the right 
    child of the computed highest frequency Node
        ex.) let NodeA to have a frequency of 3 and nodeB to have
        a frequency of 2
            *we will compute a parent Node of frequency 5, and then nodeB
            will be inserted in the left child, and NodeA in the rightChild*/

void insertLeft(Node* root, Node* insert){
    root->left = insert;
}

void insertRight(Node* root, Node* insert){
    root->right = insert;
}
    
/*function to print inorder 
void inorder(Node* root){
   if(root != NULL){
        inorder(root->left);
        printf("%d: \n", root->data);
        inorder(root->right);

    }

} 
*/

void generatecodes(Node* root, int arr[], int top){
    

    if(root->left){
        arr[top] = 0;
        generatecodes(root->left, arr, top + 1);
    }

    if(root->right){
        arr[top] = 1;
        generatecodes(root->right, arr, top + 1);
    }

    if(root->right == NULL && root->left == NULL){
        int i;
        /*printf("0x%x: ", root->data); */
        for(i=0; i < top; i++){
           root->code[i] = arr[i] + '0';   
           /*printf("%d", arr[i]);*/
        }
        root->code[top] = '\0';
        /*printf("\n"); */
    }


}

#ifdef DEBUG
int main(void){
    
    int arr[100]; /*you need 64 array slots to represent any character code
    to represent all chars in ASCII table */
    Node* root = createNode('a');
    insertRight(root, (createNode('c')));
    insertLeft(root, (createNode('b')));
    insertLeft(root->left, createNode('d'));
    insertRight(root->right, createNode('e'));
    insertLeft(root->right, createNode('f'));
    
    printCode(root, arr, 0);


return 0;
}
#endif
