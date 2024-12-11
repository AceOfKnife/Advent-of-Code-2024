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

void pushLeft(LinkedList* linkedList, long long val)
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

void pushRight(LinkedList* linkedList, long long val)
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

void pushAt(LinkedList* linkedList, long long val, int index) 
{
    if (linkedList->size <= index) {
        printf("ERROR: INDEX OUT OF BOUNDS\n");
        return;
    }
    Node* node = (Node*)malloc(sizeof(Node));
    node->val = val;

    Node* curNode = linkedList->head->next;
    int i = 0;
    while (curNode != linkedList->tail && i < index) 
    {
        curNode = curNode->next;
        i++;
    }

    Node* prevNode = curNode->prev;
    prevNode->next = node;
    curNode->prev = node;
    node->prev = prevNode;
    node->next = curNode;
    linkedList->size++;
}

long long popLeft(LinkedList* linkedList)
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
    long long val = node->val;
    free(node);
    return val;
}

long long popRight(LinkedList* linkedList)
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
    long long val = node->val;
    free(node);
    return val;
}

long long popAt(LinkedList* linkedList, int index)
{
    if (linkedList->size <= index) {
        printf("ERROR: INDEX OUT OF BOUNDS\n");
        return -1;
    }

    Node* curNode = linkedList->head->next;
    int i = 0;
    while (curNode != linkedList->tail && i < index)
    {
        curNode = curNode->next;
        i++;
    }
    Node* prevNode = curNode->prev;
    Node* nextNode = curNode->next;

    prevNode->next = nextNode;
    nextNode->prev = prevNode;

    long long val = curNode->val;
    free(curNode);
    linkedList->size--;
    return val;
}

void popVal(LinkedList* linkedList, long long val)
{
    Node* curNode = linkedList->head->next;
    while (curNode != linkedList->tail) {
        if (curNode->val == val) {
            curNode->prev->next = curNode->next;
            curNode->next->prev = curNode->prev;
            free(curNode);
            return;
        }
    }
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

void printList(LinkedList* linkedList)
{
    Node* curNode = linkedList->head->next;
    printf("[");
    while (curNode != linkedList->tail)
    {
        printf(" %lld ", curNode->val);
        curNode = curNode->next;
    }
    printf(" ]\n");
}