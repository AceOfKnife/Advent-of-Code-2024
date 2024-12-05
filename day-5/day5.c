#include "../shared/common.h"
#include "../data-structures/heap.h"

void print_arr(int *arr, int arr_size) {
    printf("[ ");
        for (int i = 0; i < arr_size; i++) {
            printf(" %d ", arr[i]);
        }
    printf("]\n");
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");

    char reader[100];
    int** crude_hash_map = (int**)malloc(sizeof(int*) * 100);
    int col_sizes[100];
    for (int i = 0; i < 100; i++) {
        crude_hash_map[i] = (int*)malloc(sizeof(int) * 100);
        col_sizes[i] = 0;
    }

    bool rules = true;
    int correct_order = 0, corrected_order = 0;

    while (fgets(reader, 100, fptr)) {
        char c = reader[0];
        if (c == '\n') {
            rules = false;
            continue;
        }
        if (rules) {
            int i = 0, prev_page = -1, next_page = -1, tmp = 0;
            while (c != '|') {
                tmp *= 10;
                tmp += c - '0';
                i++;
                c = reader[i];
            }
            prev_page = tmp;
            tmp = 0;
            i++;
            c = reader[i];
            while (c != '\n') {
                tmp *= 10;
                tmp += c - '0';
                i++;
                c = reader[i];
            }
            next_page = tmp;
            int idx = col_sizes[prev_page];
            crude_hash_map[prev_page][idx] = next_page;
            col_sizes[prev_page]++;
        } else {
            int pages[50];
            int len_pages = 0, i = 0, tmp = 0;
            while (c != '\0') {
                if (c == ',' || c == '\n') {
                    pages[len_pages] = tmp;
                    len_pages++;
                    tmp = 0;
                } else {
                    tmp *= 10;
                    tmp += c - '0';
                }
                i++;
                c = reader[i];
            }
            if (tmp != 0) {
                pages[len_pages] = tmp;
                len_pages++;
                tmp = 0;
            }
            bool correct = true;
            for (int i = 0; i < len_pages - 1; i++) {
                bool* crude_hash_set = (bool*)calloc(100, sizeof(bool));
                int page_num = pages[i];
                for (int j = 0; j < col_sizes[page_num]; j++) {
                    int next_page = crude_hash_map[page_num][j];
                    crude_hash_set[next_page] = true;
                }

                for (int k = i+1; k < len_pages; k++) {
                    correct = crude_hash_set[pages[k]];
                    if (!correct) {
                        break;
                    }
                }
                free(crude_hash_set);
                if (!correct) {
                    break;
                }
            }
            correct_order += (correct) ? pages[len_pages  / 2] : 0;
            if (!correct) {
                HeapQueue* heap = createHeapQueue();
                int tmp[100];
                bool* crude_hash_set = (bool*)calloc(100,sizeof(bool));
                for (int i = 0; i < len_pages; i++) {
                    crude_hash_set[pages[i]] = true;
                }

                for (int i = 0; i < len_pages; i++) {
                    int page_num = pages[i];
                    int count = 0;
                    for (int j = 0; j < col_sizes[page_num]; j++) {
                        int next_page = crude_hash_map[page_num][j];
                        if (crude_hash_set[next_page]) {
                            count++;
                        }
                    }
                    tmp[count] = page_num;
                    heapPush(heap, count);
                }
                int* corrected = (int*)malloc(sizeof(int) * len_pages);
                int j = len_pages - 1;

                while (heapSize(heap)) {
                    int key = heapPop(heap);
                    int val = tmp[key];
                    corrected[j] = val;
                    j--;
                }

                corrected_order += corrected[len_pages / 2];
                freeHeap(heap);
                free(corrected);
                free(crude_hash_set);
            }
        }
    }

    for (int i = 0; i < 100; i++) {
        free(crude_hash_map[i]);
    }
    free(crude_hash_map);
    fclose(fptr);

    printf("Correct order updates: %d\n", correct_order);
    printf("Corrected order updates: %d\n", corrected_order);

    return 0;
}