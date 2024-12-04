#include "../shared/common.h"

void print_arr_2d(char arr[][10], int row) {
    for (int j = 0; j < row; j++) {
        printf("[ ");
            for (int i = 0; i < 10; i++) {
                printf(" %c ", arr[j][i]);
            }
        printf("]\n");
    }
}

void print_arr(char *arr, int arr_size) {
    printf("[ ");
        for (int i = 0; i < arr_size; i++) {
            printf(" %c ", arr[i]);
        }
    printf("]\n");
}

void reverse_string(char *s) {
    int len = strlen(s);
    int i = 0, j = len - 1;
    while (i < j) {
        char tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        i++;
        j--;
    }
}

int find_all_occurrences(char *s) {
    int count = 0;
    char *current = s;
    char xmas[] = "XMAS";

    while ((current = strstr(current, xmas)) != NULL) {
        current++;
        count++;
    }

    current = s;
    reverse_string(current);

    while ((current = strstr(current, xmas)) != NULL) {
        current++;
        count++;
    }

    return count;
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    int row = 140, col = 140;
    char reader[5000];
    char input_arr[row][col];
    int i, j = 0;

    while (fgets(reader, 5000, fptr)) {
        char c = reader[0];
        int i = 0;
        while (i < col) {
            c = reader[i];
            input_arr[j][i] = c;
            i++;
        }
        j++;
    }

    int count = 0;

    // Look for horizontal XMAS
    for (i = 0; i < row; i++) {
        char* s = (char*)malloc(sizeof(char) * (col + 1));
        for (j = 0; j < col; j++) {
            s[j] = input_arr[i][j];
        }
        s[col] = '\0';
        count += find_all_occurrences(s);
        free(s);
    }

    // Look for vertical XMAS
    for (i = 0; i < col; i++) {
        char* s = (char*)malloc(sizeof(char) * (row + 1));
        for (j = 0; j < row; j++) {
            s[j] = input_arr[j][i];
        }
        s[row] = '\0';
        count += find_all_occurrences(s);
        free(s);
    }

    // Look for top left -> bottom right diagonal XMAS
    for (int i = 3; i < row; i++) {
        char* s = (char*)malloc(sizeof(char) * (i+2));
        for (j = 0; j <= i; j++) {
            s[j] = input_arr[i-j][j];
        }
        s[i+1] = '\0';
        count += find_all_occurrences(s);
        free(s);
    }
    for (int j = 1; j < col - 3; j++) {
        int len = col - j;
        char* s = (char*)malloc(sizeof(char) * (len+1));
        for (int i = 0; i < len; i++) {
            s[i] = input_arr[row-i-1][j+i]; 
        }
        s[len] = '\0';
        count += find_all_occurrences(s);
        free(s);
    }

    // Look for top right -> bottom left diagonal XMAS
    for (int i = 3; i < row; i++) {
        char* s = (char*)malloc(sizeof(char) * (i+2));
        int idx = 0;
        for (j = col - 1; j >= col-i-1; j--) {
            s[idx] = input_arr[i-idx][j];
            idx++;
        }
        s[i+1] = '\0';
        count += find_all_occurrences(s);
        free(s);
    }
    for (int j = 3; j < col-1; j++) {
        char* s = (char*)malloc(sizeof(char) * (j+2));
        for (int i = 0; i < j + 1; i++) {
            s[i] = input_arr[row-i-1][j-i]; 
        }
        s[j+1] = '\0';
        count += find_all_occurrences(s);
        free(s);
    }

    // Part 2
    int mas_count = 0;
    for (int i = 1; i < row - 1; i++) {
        for (int j = 1; j < col - 1; j++) {
            if (input_arr[i][j] == 'A') {
                bool first_diag = (input_arr[i-1][j-1] == 'M' && input_arr[i+1][j+1] == 'S') || (input_arr[i-1][j-1] == 'S' && input_arr[i+1][j+1] == 'M');
                bool second_diag = (input_arr[i+1][j-1] == 'M' && input_arr[i-1][j+1] == 'S') || (input_arr[i+1][j-1] == 'S' && input_arr[i-1][j+1] == 'M');
                mas_count += (first_diag && second_diag) ? 1 : 0;
            }
        }
    }

    printf("XMAS count: %d\n", count);
    printf("X-MAS count: %d\n", mas_count);
    fclose(fptr);
    return 0;
}