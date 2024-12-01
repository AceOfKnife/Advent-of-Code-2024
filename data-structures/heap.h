#ifndef HEAP_H
#define HEAP_H

#include "../shared/common.h"

typedef struct
{
    int* arr;
    int size;
    int maxSize;
} HeapQueue;

HeapQueue* createHeapQueue();

void heapPush(HeapQueue* heap, int val);

int heapPop(HeapQueue* heap);

int heapSize(HeapQueue* heap);

void freeHeap(HeapQueue* heap);

#endif