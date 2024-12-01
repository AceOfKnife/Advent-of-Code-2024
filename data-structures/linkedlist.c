#include "../shared/common.h"
#include "node.h"
#include "linkedlist.h"

LinkedList* createLinkedList()
{
    LinkedList* linkedList = (LinkedList*)malloc(sizeof(LinkedList));
    if (!linkedList) 
    {
        return NULL;
    }

    linkedList->size = 0;
    Node* head = (Node*)malloc(sizeof(Node));
    Node* tail = (Node*)malloc(sizeof(Node));
    
    head->prev = NULL;
    head->next = tail;
    tail->prev = head;
    tail->next = NULL;

    linkedList->head = head;
    linkedList->tail = tail;

    return linkedList;
}

void pushLeft(LinkedList* linkedList, int val)
{
    Node* node = (Node*)malloc(sizeof(Node)); 
    Node* prevHead = linkedList->head->next;
    
    node->val = val;
    node->next = prevHead;
    node->prev = linkedList->head;

    linkedList->head->next = node;
    prevHead->prev = node;
    linkedList->size++;
}

void pushRight(LinkedList* linkedList, int val)
{
    Node* node = (Node*)malloc(sizeof(Node)); 
    Node* prevTail = linkedList->tail->prev;
    
    node->val = val;
    node->next = linkedList->tail;
    node->prev = prevTail;

    linkedList->tail->prev = node;
    prevTail->next = node;
    linkedList->size++;
}

int popLeft(LinkedList* linkedList)
{
    if (linkedList->size == 0) 
    {
        printf("ERROR: NO ITEMS IN LIST\n");
        return -1;
    }

    Node* node = linkedList->head->next;
    linkedList->head->next = node->next;
    node->next->prev = linkedList->head;
    linkedList->size--;
    int val = node->val;
    free(node);
    return val;
}

int popRight(LinkedList* linkedList)
{
    if (linkedList->size == 0) 
    {
        printf("ERROR: NO ITEMS IN LIST\n");
        return -1;
    }

    Node* node = linkedList->tail->prev;
    linkedList->tail->prev = node->prev;
    node->prev->next = linkedList->tail;
    linkedList->size--;
    int val = node->val;
    free(node);
    return val;
}

int getSize(LinkedList* linkedList)
{
    return linkedList->size;
}

void freeList(LinkedList* linkedList)
{
    Node* curNode = linkedList->head;

    while (curNode != NULL)
    {
        Node* freeNode = curNode;
        curNode = curNode->next;
        free(freeNode);
    }
    
    free(linkedList);
}