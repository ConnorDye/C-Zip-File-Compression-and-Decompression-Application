/*
 * Defines functions for linked lists.
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"

Node* insertRecursive(Node* head,int pos, Node* node);

/* lstcreate: Creates an empty linked list. */
List *lstcreate() {
    List* list;
    if(!(list = (List *)malloc(sizeof(List))))
        perror("mem");
    list->head = NULL;
    list->size = 0;
    return list; 
    /* ...returns a pointer to a new, dynamically allocated List structure. */
}

/* lstdestroy: Destroys an existing linked list. */
void lstdestroy(List *lst) {
    while(lst->head != NULL){
        Node* temp = lst->head;
        lst->head = lst->head->next;
        free(temp);
    }
    free(lst);

   /* FUNCTIONALITY:
    *  lst - A pointer to a List structure
    * ...deallocate all memory associated with "lst". */
}

/* lstsize: Computes the size of a linked list. */
unsigned int lstsize(List *lst) {
    return lst->size;
   /* FUNCTIONALITY
    *  lst - A pointer to a List structure
    * ...return the number of nodes in "lst". */
}

/* lstget: Gets an element in a linked list at an index. */
Node* lstget(List *lst, unsigned int idx) {
    Node* temp = lst->head;
    if(idx < 0 || idx > lst->size -1 || lst ->size == 0)
        return NULL;

    while(temp !=NULL && idx != 0){
        temp = temp->next;
        idx--;
    }

    return temp; /*originally temp */
   /* FUNCTIONALITY:
    *  lst - A pointer to a List structure
    *  idx - A non-negative index
    * ...return a pointer to element "idx" of "lst", NULL if "idx" outside
    * bounds [0, size - 1] */
}

/* lstset: Sets an element in a linked list at an index. */
int lstset(List *lst, unsigned int idx, int data) {
    Node* temp = lst->head;
    if(idx < 0 || idx > lst->size -1 || lst ->size == 0)
        return 1;

    while(temp !=NULL && idx != 0){
        temp = temp->next;
        idx--;
    }

    temp->data = data;
    return 0;
   /* FUNCTIONALITY
    *  lst  - A pointer to a List structure
    *  idx  - A non-negative index
    *  data - A pointer to a desired element
    * ...set element "idx" of "lst" to "data"; return 0 on success, 1 if "idx"
    * outside bounds [0, size - 1]. */
}


/* lstadd: Adds an element to a linked list at an index. */
int lstadd(List *lst, unsigned int idx, Node *newNode) {
    Node* temp = lst->head;
    if(idx < 0 || idx > lst->size)
    {
        return 1; /*if out of bounds return 1 */
    }
    else{
        lst->head = insertRecursive(temp, idx, newNode);
        lst->size = lst->size + 1;
        return 0;
    }


}


/* lstremove: Removes an element from a linked list at an index. 
    *returns 1 if failed
    *returns 0 upon success
    */
int lstremove(List *lst, unsigned int idx) {
    /*void* dataReturn = NULL; */
    /*Node* temp = lst->head; */
    if(idx < 0 || idx > lst->size - 1 || lst->size == 0){
        return 1;
    }
    else{
        Node *temp = lst->head, *prev;
        int count = 0;

        if (temp != NULL && idx == count) {
            lst->head = temp->next; 
            /*dataReturn = temp->data;*/
            /*free(temp);*/
            lst->size = lst->size - 1;
            return 0;
        }

        while (temp != NULL && idx != count) {
            prev = temp;
            temp = temp->next;
            count++;
        }

        if (temp == NULL)
            return 1;

        prev->next = temp->next;
        /*dataReturn = temp->data; */
        /*free(temp);*/
        lst->size = lst->size - 1;
        return 0;
    }


   /* FUNCTIONALITY
    *  lst  - A pointer to a List structure
    *  idx  - A non-negative index
    * ...remove element "idx" of "lst", shifting remaining elements as
    * necessary; return a pointer to the removed element, NULL if "idx"
    * outside bounds [0, size - 1]. */
}

/*helper function for insert recursive */
Node* insertRecursive(Node* head,int pos, Node* newNode)
{
    if(pos==0 || head==NULL)
    {
        newNode->next=head;
        head=newNode;
        return head;
    }
    else
        head->next = insertRecursive(head->next,pos-1, newNode);

    return head;

}



Node* insert(Node *head, int idx, Node* nn){
    if(idx==0 || head == NULL){
        nn->next = head;
        head = nn;
    }
    else{
        head->next = insert(head->next, idx - 1, nn);
    }

    return head;
}

