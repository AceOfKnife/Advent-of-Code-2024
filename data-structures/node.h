#ifndef NODE_H
#define NODE_H

#include "../shared/common.h"

typedef struct Node 
{
    int val;
    struct Node* next;
    struct Node* prev;
} Node;

#endif