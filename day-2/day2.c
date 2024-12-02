#include "../shared/common.h"

void print_arr(int* arr, int arr_size) {
    printf("[ ");
    for (int i = 0; i < arr_size; i++) {
        printf(" %d ", arr[i]);
    }
    printf("]\n");
}

bool comparator(int a, int b, int mode) {
    int lower, higher;
    if (mode == 1) {
        lower = 1;
        higher = 3;
    } else {
        lower = -3;
        higher = -1;
    }
    return (b - a) >= lower && (b - a) <= higher;
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");

    char reader[100];

    int safe = 0, saved = 0;
    int* nums = (int*)malloc(sizeof(int) * 10);

    while (fgets(reader, 100, fptr)) {
        char c = reader[0];
        int i = 0, j = 0, lower = -1, higher = -1, prev_num = -1, cur_num = 0;
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

            int lis_l = 1, lds_l = 1;
            for (i = 1; i < nums_count; i++) {
                int diff = nums[i] - nums[i-1];
                if (diff >= 1 && diff <= 3) {
                    lis_l++;
                } else {
                    break;
                }
            }
            for (i = 1; i < nums_count; i++) {
                int diff = nums[i] - nums[i-1];
                if (diff >= -3 && diff <= -1) {
                    lds_l++;
                } else {
                    break;
                }
            }

            int lds_r = 1, lis_r = 1;
            for (i = nums_count - 2; i >= 0; i--) {
                int diff = nums[i] - nums[i+1];
                if (diff >= -3 && diff <= -1) {
                    lds_r++;
                } else {
                    break;
                }
            }
            for (i = nums_count - 2; i >= 0; i--) {
                int diff = nums[i] - nums[i+1];
                if (diff >= 1 && diff <= 3) {
                    lis_r++;
                } else {
                    break;
                }
            }

            int len_increase = lis_l + lds_r, len_decrease = lds_l + lis_r;

            int combined_len = (len_increase > len_decrease) ? len_increase : len_decrease;
            int left = (len_increase > len_decrease) ? lis_l - 1 : lds_l - 1;
            int right = (len_increase > len_decrease) ? nums_count - lds_r : nums_count - lis_r;
            int mode = len_increase > len_decrease;

            if (combined_len == nums_count - 1) {
                saved += (comparator(nums[left], nums[right], mode)) ? 1 : 0;
            }

            if (combined_len == nums_count) {
                if (left == 0 || right == nums_count - 1) {
                    saved++;
                } else {
                    saved += (comparator(nums[left-1], nums[right], mode) || comparator(nums[left], nums[right+1], mode)) ? 1 : 0;
                }
            }
        }
    }
    free(nums);
    printf("Safe reports: %d\n", safe);
    printf("Saved reports: %d\n", saved);
    return 0;
}