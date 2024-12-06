#include "../shared/common.h"
#include "../data-structures/linkedlist.h"

#define FILE_PATH "./input.txt"

void print_arr(int *arr, int arr_size) {
    printf("[ ");
        for (int i = 0; i < arr_size; i++) {
            printf(" %d ", arr[i]);
        }
    printf("]\n");
}

void print_arr_2d(char** arr, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        printf("[ ");
            for (int j = 0; j < cols; j++) {
                printf(" %c ", arr[i][j]);
            }
        printf("]\n");
    }
}

char** get_map(int rows, int cols, int* guard_x, int* guard_y) {
    FILE *fptr = fopen(FILE_PATH, "r");
    char reader[200];
    char** map = (char**)malloc(sizeof(char*) * rows);
    for (int i = 0; i < cols; i++) {
        map[i] = (char*)malloc(sizeof(char) * cols);
    }
    for (int i = 0; i < rows; i++) {
        fgets(reader, 200, fptr);
        for (int j = 0; j < cols; j++) {
            if (reader[j] == '^') {
                *guard_x = i, *guard_y = j;
                map[i][j] = '.';
            } else {
                map[i][j] = reader[j];
            }
        }
    }
    fclose(fptr);
    return map;
}

bool infinite_loop(char** map, int rows, int cols, int start_x, int start_y) {
    int directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};

    // Fast and slow pointers
    int fast_direction = 0, slow_direction = 0;
    int fast_x = start_x, fast_y = start_y;
    int slow_x = start_x, slow_y = start_y;

    int offset = 0;
    while (fast_x >= 0 && fast_x < rows && fast_y >= 0 && fast_y < cols) {

        int fast_new_x = fast_x + directions[fast_direction][0];
        int fast_new_y = fast_y + directions[fast_direction][1];

        int slow_new_x = slow_x + directions[slow_direction][0];
        int slow_new_y = slow_y + directions[slow_direction][1];

        if (fast_new_x >= 0 && fast_new_x < rows && fast_new_y >= 0 && fast_new_y < cols && map[fast_new_x][fast_new_y] == '#') {
            fast_direction = (fast_direction + 1) % 4;
        } else {
            fast_x = fast_new_x, fast_y = fast_new_y;
        }

        if (offset % 2 == 1) {
            if (slow_new_x >= 0 && slow_new_x < rows && slow_new_y >= 0 && slow_new_y < cols && map[slow_new_x][slow_new_y] == '#') {
                slow_direction = (slow_direction + 1) % 4;
            } else {
                slow_x = slow_new_x, slow_y = slow_new_y;
            }
        }

        if (fast_x == slow_x && fast_y == slow_y && fast_direction == slow_direction) {
            return true;
        }

        offset++;
    }

    return false;

}

int main() {
    int rows = 130; // 130 input, 10 test
    int cols = 130; // 130 input, 10 test

    int guard_x, guard_y;
    char** map = get_map(rows, cols, &guard_x, &guard_y);
    int start_x = guard_x, start_y = guard_y;

    int distinct_positions = 0;
    int directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    int direction = 0; // 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT

    while (guard_x >= 0 && guard_x < rows && guard_y >= 0 && guard_y < cols) {
        if (map[guard_x][guard_y] == '.') {
            distinct_positions++;
            map[guard_x][guard_y] = 'X';
        }
        int new_x = guard_x + directions[direction][0];
        int new_y = guard_y + directions[direction][1];
        if (new_x >= 0 && new_x < rows && new_y >= 0 && new_y < cols && map[new_x][new_y] == '#') {
            direction = (direction + 1) % 4;
        } else {
            guard_x = new_x, guard_y = new_y;
        }
    }

    // Part 2
    int obstructions = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == start_x && j == start_y) {
                continue;
            }
            if (map[i][j] == '.' || map[i][j] == 'X') {
                map = get_map(rows, cols, &guard_x, &guard_y);
                map[i][j] = '#';
                obstructions += infinite_loop(map, rows, cols, start_x, start_y);
            }
        }
    }

    free(map);
    printf("Number of distinct positions: %d\n", distinct_positions);
    printf("Number of obstructable positions: %d\n", obstructions);
    return 0;
}