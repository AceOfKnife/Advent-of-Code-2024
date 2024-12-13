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

void pushLeft(LinkedList* linkedList, long long val);

void pushRight(LinkedList* linkedList, long long val);

void pushRightNode(LinkedList* linkedList, Node* node);

void pushAt(LinkedList* linkedList, long long val, int index);

long long popLeft(LinkedList* linkedList);

Node* popLeftNode(LinkedList* linkedList);

long long popRight(LinkedList* linkedList);

long long popAt(LinkedList* linkedList, int index);

void popVal(LinkedList* linkedList, long long val);

int getSize(LinkedList* linkedList);

void freeList(LinkedList* linkedList);

void printList(LinkedList* linkedList);

#endif