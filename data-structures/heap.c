#include "heap.h"
#include "../shared/common.h"

HeapQueue* createHeapQueue()
{
    HeapQueue* heap = (HeapQueue*)malloc(sizeof(HeapQueue));
    if (!heap) 
    {
        printf("Failed to allocate memory.\n");
        return NULL;
    }

    heap->maxSize = 100;
    heap->size = 0;
    heap->arr = (int*)malloc(sizeof(int) * heap->maxSize);

    return heap;
}

void heapPush(HeapQueue* heap, int val)
{
    if (heap->size == heap->maxSize)
    {
        heap->maxSize *= 2;
        int* temp = (int*)realloc(heap->arr, sizeof(int) * heap->maxSize);
        if (!temp)
        {
            fprintf(stderr, "Failed to reallocate.\n");
            return;
        }
        heap->arr = temp;
    }

    int index = heap->size;
    heap->arr[index] = val;
    int parent = (index - 1) / 2;

    while (parent >= 0 && heap->arr[parent] > heap->arr[index])
    {
        heap->arr[index] = heap->arr[parent];
        heap->arr[parent] = val;
        index = parent;
        parent = (index - 1) / 2;
    }

    heap->size++;
}

int heapPop(HeapQueue* heap)
{
    if (heap->size == 0)
    {
        printf("No items.\n");
        return -1;
    }

    int val = heap->arr[0];
    heap->size--;
    
    if (heap->size == 0)
    {
        return val;
    }
    
    int index = 0;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;
    heap->arr[index] = heap->arr[heap->size];

    while (leftChild < heap->size)
    {
        int replaceIndex = leftChild;

        if (rightChild < heap->size && heap->arr[rightChild] < heap->arr[leftChild])
        {
            replaceIndex = rightChild;
        }

        if (heap->arr[index] <= heap->arr[replaceIndex])
        {
            break;
        }

        // Swap values
        int temp = heap->arr[index];
        heap->arr[index] = heap->arr[replaceIndex];
        heap->arr[replaceIndex] = temp;

        // Update indices
        index = replaceIndex;
        leftChild = 2 * index + 1;
        rightChild = 2 * index + 2;
    }

    return val;
}

int heapSize(HeapQueue* heap)
{
    return heap->size;
}

void freeHeap(HeapQueue* heap)
{
    free(heap->arr);
    free(heap);
}