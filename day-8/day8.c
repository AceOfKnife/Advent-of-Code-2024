#include "../shared/common.h"

void print_arr_2d(bool** arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("[ ");
            for (int j = 0; j < size; j++) {
                printf(" %d ", arr[i][j]);
            }
        printf("]\n");
    }
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    char reader[100];
    int size = 50; // 12 test, 50 input
    bool** antinode = (bool**)malloc(sizeof(bool*) * size);
    for (int i = 0; i < size; i++) {
        antinode[i] = (bool*)calloc(size, sizeof(bool));
    }
    int*** crude_hash_map = (int***)malloc(sizeof(int**) * 128);
    int* lengths = (int*)calloc(128, sizeof(int));
    for (int i = 0; i < 128; i++) {
        crude_hash_map[i] = (int**)malloc(sizeof(int*) * 50);
        for (int j = 0; j < 50; j++) {
            crude_hash_map[i][j] = (int*)malloc(sizeof(int) * 2);
        }
    }
    int row = 0;
    while (fgets(reader, 100, fptr)) {
        char c = reader[0];
        int col = 0;
        while (c != '\0' && c != '\n') {
            if (c != '.') {
                int index = lengths[(int)c];
                crude_hash_map[(int)c][index][0] = row;
                crude_hash_map[(int)c][index][1] = col;
                lengths[(int)c]++;
            }
            col++;
            c = reader[col];
        }
        row++;
    }

    int antinode_count = 0;

    for (int i = 0; i < 128; i++) {
        if (lengths[i] == 0) {
            continue;
        }
        for (int j = 0; j < lengths[i] - 1; j++) {
            for (int k = j + 1; k < lengths[i]; k++) {
                int first_x = crude_hash_map[i][j][0], first_y = crude_hash_map[i][j][1];
                int second_x = crude_hash_map[i][k][0], second_y = crude_hash_map[i][k][1];
                int dx = second_x - first_x, dy = second_y - first_y;

                int forward_x = second_x, forward_y = second_y;
                int backward_x = first_x, backward_y = first_y;

                // Part 1

                // if (forward_x >= 0 && forward_x < size && forward_y >= 0 && forward_y < size && !antinode[forward_x][forward_y]) {
                //     antinode_count++;
                //     antinode[forward_x][forward_y] = true;
                // }

                // if (backward_x >= 0 && backward_x < size && backward_y >= 0 && backward_y < size && !antinode[backward_x][backward_y]) {
                //     antinode_count++;
                //     antinode[backward_x][backward_y] = true;
                // }

                // Part 2

                while (forward_x >= 0 && forward_x < size && forward_y >= 0 && forward_y < size) {
                    if (!antinode[forward_x][forward_y]) {
                        antinode_count++;
                        antinode[forward_x][forward_y] = true;
                    }
                    forward_x += dx, forward_y += dy;
                }

                while (backward_x >= 0 && backward_x < size && backward_y >= 0 && backward_y < size) {
                    if (!antinode[backward_x][backward_y]) {
                        antinode_count++;
                        antinode[backward_x][backward_y] = true;
                    }
                    backward_x -= dx, backward_y -= dy;
                }
            }
        }
    }
    printf("Unique antinodes: %d\n", antinode_count);

    for (int i = 0; i < size; i++) {
        free(antinode[i]);
    }
    free(antinode);
    for (int i = 0; i < 128; i++) {
        for (int j = 0; j < 50; j++) {
            free(crude_hash_map[i][j]);
        }
        free(crude_hash_map[i]);
    }
    free(crude_hash_map);
    free(lengths);
    fclose(fptr);
    return 0;
}