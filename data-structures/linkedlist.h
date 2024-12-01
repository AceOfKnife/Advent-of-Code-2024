#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "../shared/common.h"
#include "node.h"

typedef struct
{
    Node* head;
    Node* tail;
    int size;
} LinkedList;

LinkedList* createLinkedList();

void pushLeft(LinkedList* linkedList, int val);

void pushRight(LinkedList* linkedList, int val);

int popLeft(LinkedList* linkedList);

int popRight(LinkedList* linkedList);

int getSize(LinkedList* linkedList);

void freeList(LinkedList* linkedList);

#endif