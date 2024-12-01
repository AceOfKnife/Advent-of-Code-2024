#include "../shared/common.h"
#include "../data-structures/heap.h"

int main()
{
    FILE *fptr = fopen("./input.txt", "r");

    char reader[100];
    int* crudeHashMap = (int*)calloc(100000, sizeof(int));

    HeapQueue* heap1 = createHeapQueue();
    HeapQueue* heap2 = createHeapQueue();

    while(fgets(reader, 100, fptr))
    {
        int val1 = 0, val2 = 0, i = 0;
        char c = '0';
        while (c != ' ')
        {
            val1 *= 10;
            val1 += c - '0';
            c = reader[i];
            i++;
        }
        while (c == ' ')
        {
            i++;
            c = reader[i];
        }
        while (c != '\n')
        {
            val2 *= 10;
            val2 += c - '0';
            i++;
            c = reader[i];
        }
        heapPush(heap1, val1);
        heapPush(heap2, val2);
        crudeHashMap[val2]++;;
    }
    int totalDistance = 0, similarity = 0;

    while (heapSize(heap1) && heapSize(heap2))
    {
        int val1 = heapPop(heap1);
        int val2 = heapPop(heap2);
        totalDistance += abs(val1 - val2);
        similarity += val1 * crudeHashMap[val1];
    }

    fclose(fptr);
    freeHeap(heap1);
    freeHeap(heap2);
    free(crudeHashMap);

    printf("Total Distance is: %d\n", totalDistance);
    printf("Similarity Score: %d\n", similarity);

    return 0;
}