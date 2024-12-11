#ifndef NODE_H
#define NODE_H

#include "../shared/common.h"

typedef struct Node 
{
    long long val;
    struct Node* next;
    struct Node* prev;
} Node;

#endif