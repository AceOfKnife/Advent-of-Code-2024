#include "../shared/common.h"

void print_arr(int *arr, int arr_size) {
    printf("[ ");
        for (int i = 0; i < arr_size; i++) {
            printf(" %d ", arr[i]);
        }
    printf("]\n");
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    char reader[50000];
    int size = 0, capacity = 50000;
    int* input_arr = (int*)malloc(sizeof(int) * capacity);
    unsigned long long checksum = 0;
    
    fgets(reader, 50000, fptr);

    char c = reader[0];
    int i = 0;

    while (c != '\0') {
        int digit = c - '0';
        if (i % 2 == 0) {
            int id = i / 2;
            for (int j = 0; j < digit; j++) {
                input_arr[size] = id;
                size++;
                if (size >= capacity) {
                    capacity *= 2;
                    input_arr = realloc(input_arr, sizeof(int) * capacity);
                    if (!input_arr) {
                        perror("Failed to allocate memory");
                        exit(1);
                    }
                }
            }
        } else {
            for (int j = 0; j < digit; j++) {
                input_arr[size] = -1;
                size++;
                if (size >= capacity) {
                    capacity *= 2;
                    input_arr = realloc(input_arr, sizeof(int) * capacity);
                    if (!input_arr) {
                        perror("Failed to allocate memory");
                        exit(1);
                    }
                }
            }
        }
        i++;
        c = reader[i];
    }

    int j = size-1;

    // Part 1
    // for (int i = 0; i < size; i++) {
    //     if (input_arr[i] != -1) {
    //         continue;
    //     }
    //     while (j >= 0 && input_arr[j] == -1) {
    //         j--;
    //     }
    //     if (i >= j) {
    //         break;
    //     }
    //     input_arr[i] = input_arr[j];
    //     input_arr[j] = -1;
    // }

    // Part 2

    while (j >= 0 && input_arr[j] == -1) {
        j--;
    }

    int id_max = input_arr[j];
    
    for (int i = id_max; i >= 0; i--) {
        while (j > 0 && input_arr[j] != i) {
            j--;
        }
        int id_end = j;
        while (j > 0 && input_arr[j-1] == i) {
            j--;
        }
        int id_start = j;
        int start = -1, end = -1;
        for (int k = 0; k < j; k++) {
            if (input_arr[k] == -1) {
                if (start == -1) {
                    start = k;
                    end = k+1;
                } else {
                    end++;
                }
            } else {
                start = -1;
                end = -1;
            }
            if (end - start == id_end - id_start + 1) {
                while (start < end) {
                    input_arr[start] = i;
                    start++;
                }
                while (id_start <= id_end) {
                    input_arr[id_start] = -1;
                    id_start++;
                }
                break;
            }
        }
    }


    for (int i = 0; i < size; i++) {
        if (input_arr[i] == -1) {
            continue;
        }
        checksum += input_arr[i] * i;
    }
    free(input_arr);
    fclose(fptr);
    printf("Checksum: %llu\n", checksum);
    return 0;
}