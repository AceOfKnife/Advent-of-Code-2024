#include "../shared/common.h"

void print_arr_2d(int** arr, int rows, int* cols) {
    for (int i = 0; i < rows; i++) {
        printf("[ ");
            for (int j = 0; j < cols[i]; j++) {
                printf(" %d ", arr[i][j]);
            }
        printf("]\n");
    }
}

bool dfs(int i, long* nums, int nums_length, int sum, int target) {
    if (i == nums_length) {
        return sum == target;
    }
    return dfs(i+1, nums, nums_length, sum + nums[i], target) || dfs(i+1, nums, nums_length, sum * nums[i], target);
}

long concat_long_numbers(long num1, long num2) {
    char str1[25], str2[25], resultStr[50]; 
    
    sprintf(str1, "%ld", num1);
    sprintf(str2, "%ld", num2);
    
    strcpy(resultStr, str1);
    strcat(resultStr, str2);
    
    long result = atol(resultStr);
    return result;
}

bool dfs_2(int i, long* nums, int nums_length, long sum, long target) {
    if (i == nums_length) {
        return sum == target;
    }
    return dfs_2(i+1, nums, nums_length, sum + nums[i], target) || 
           dfs_2(i+1, nums, nums_length, sum * nums[i], target) || 
           dfs_2(i+1, nums, nums_length, concat_long_numbers(sum, nums[i]), target);
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    char reader[100];
    int rows = 850; // 850 input, 9 test
    long** input_nums = (long**)malloc(sizeof(long*) * rows);
    int* col_sizes = (int*)calloc(rows, sizeof(int));
    for (int i = 0; i < rows; i++) {
        input_nums[i] = (long*)malloc(sizeof(long*) * 20);
    }
    for (int i = 0; i < rows; i++) {
        fgets(reader, 100, fptr);
        char c = reader[0];
        long num = 0, j = 0;
        while (c != '\0') {
            if (c == ':' || c == ' ' ||  c == '\n') {
                if (c == ':') {
                    j++;
                }
                input_nums[i][col_sizes[i]] = num;
                col_sizes[i]++;
                num = 0;
            } else {
                num *= 10;
                num += c - '0';
            }
            j++;
            c = reader[j];
        }
    }
    
    long result = 0, new_result = 0;

    for (int i = 0; i < rows; i++) {
        if (dfs(1, input_nums[i], col_sizes[i], 0, input_nums[i][0])) {
            result += input_nums[i][0];
        }
        if (dfs_2(1, input_nums[i], col_sizes[i], 0, input_nums[i][0])) {
            new_result += input_nums[i][0];
        }
    }

    for (int i = 0; i < rows; i++) {
        free(input_nums[i]);
    }
    free(input_nums);
    free(col_sizes);
    fclose(fptr);

    printf("Total calibration result: %ld\n", result);
    printf("New total calibration result: %ld\n", new_result);
    return 0;
}