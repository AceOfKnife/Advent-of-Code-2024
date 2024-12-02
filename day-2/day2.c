#include "../shared/common.h"

void print_arr(int* arr, int arr_size) {
    printf("[ ");
    for (int i = 0; i < arr_size; i++) {
        printf(" %d ", arr[i]);
    }
    printf("]\n");
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");

    char reader[100];

    int safe = 0, saved = 0;

    while (fgets(reader, 100, fptr)) {
        char c = reader[0];
        int i = 0, j = 0, lower = -1, higher = -1, prev_num = -1, cur_num = 0;
        int* nums = (int*)malloc(sizeof(int) * 10);
        bool safe_flag = true;
        while (c != '\0') {
            if (c == ' ' || c == '\n') {
                if (prev_num == -1) {
                    prev_num = cur_num;
                } else {
                    int diff = cur_num - prev_num;
                    if (lower == -1) {
                        lower = (diff > 0) ? 1 : -3;
                        higher = (diff > 0) ? 3 : -1;
                    }
                    if (diff < lower || diff > higher) {
                        safe_flag = false;
                    }
                    prev_num = cur_num;
                }
                nums[j] = cur_num;
                j++;
                cur_num = 0;
            } else {
                cur_num *= 10;
                cur_num += c - '0';
            }
            i++;
            c = reader[i];
        }
        if (safe_flag) {
            safe++;
            saved++;
        } else {
            int nums_count = j;
            for (i = 0; i < nums_count; i++) {
                bool valid = true;
                lower = -1, higher = -1, prev_num = -1;
                for (j = 0; j < nums_count; j++) {
                    if (i == j) {
                        continue;
                    }
                    if (prev_num == -1) {
                        prev_num = nums[j];
                    } else {
                        int diff = nums[j] - prev_num;
                        if (lower == -1) {
                            lower = (diff > 0) ? 1 : -3;
                            higher = (diff > 0) ? 3 : -1;
                        }
                        if (diff < lower || diff > higher) {
                            valid = false;
                            break;
                        }
                        prev_num = nums[j];
                    }
                }
                if (valid) {
                    saved++;
                    break;
                }
            }
        }
        free(nums);
    }

    printf("Safe reports: %d\n", safe);
    printf("Saved reports: %d\n", saved);
    return 0;
}