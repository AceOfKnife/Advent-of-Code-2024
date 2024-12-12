#include "../shared/common.h"

void print_arr_2d(char** arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("[ ");
            for (int j = 0; j < size; j++) {
                printf(" %c ", arr[i][j]);
            }
        printf("]\n");
    }
}

void dfs(char** map, bool** seen, int size, char plant, int x, int y, int* result) {
    int directions[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    seen[x][y] = true;
    result[0]++;
    int perimeter = 0;
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0], dy = directions[i][1];
        int ddx = x + dx, ddy = y + dy;
        if (ddx < 0 || ddx >= size || ddy < 0 || ddy >= size || map[ddx][ddy] != plant) {
            perimeter++;
        }

        if (ddx >= 0 && ddx < size && ddy >= 0 && ddy < size && map[ddx][ddy] == plant && !seen[ddx][ddy]) {
            dfs(map, seen, size, plant, ddx, ddy, result);
        }
    }
    result[1] += perimeter;
}

void dfs_2(char** map, bool** seen, int size, char plant, int x, int y, int* result) {
    int directions[4][2] = {{-1,0}, {1,0}, {0,1}, {0,-1}};
    seen[x][y] = true;
    result[0]++;
    int adjacent = 0;
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0], dy = directions[i][1];
        int ddx = x + dx, ddy = y + dy;

        if (ddx >= 0 && ddx < size && ddy >= 0 && ddy < size && map[ddx][ddy] == plant && !seen[ddx][ddy]) {
            dfs_2(map, seen, size, plant, ddx, ddy, result);
        }

        if (ddx >= 0 && ddx < size && ddy >= 0 && ddy < size && map[ddx][ddy] == plant) {
            adjacent++;
        }
    }
    // Counting corners!
    if (adjacent == 0) {
        result[1] += 4;
    } else if (adjacent == 1) {
        result[1] += 2;
    } else if (adjacent == 2) {
        int pos[2][2];
        int idx = 0;
        for (int i = 0; i < 4; i++) {
            int dx = directions[i][0], dy = directions[i][1];
            int ddx = x + dx, ddy = y + dy;
            if (ddx >= 0 && ddx < size && ddy >= 0 && ddy < size && map[ddx][ddy] == plant) {
                pos[idx][0] = dx;
                pos[idx][1] = dy;
                idx++;
            }
        }

        // Check that the two adjacent pieces are perpendicular to each other
        if (abs(pos[0][0]) != abs(pos[1][0])) {
            int ddx = x + pos[0][0];
            int ddy = y + pos[1][1];
            if (map[ddx][ddy] != plant) {
                result[1] += 1;
            }
            result[1] += 1;
        }
    } else if (adjacent == 3) {
        int pos[3][2];
        int idx = 0;
        int new_directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
        for (int i = 0; i < 4; i++) {
            int dx = new_directions[i][0], dy = new_directions[i][1];
            int ddx = x + dx, ddy = y + dy;
            if (ddx >= 0 && ddx < size && ddy >= 0 && ddy < size && map[ddx][ddy] == plant) {
                pos[idx][0] = dx;
                pos[idx][1] = dy;
                idx++;
            }
        }
        for (int i = 0; i < 2; i++) {
            for (int j = i; j < 3; j++) {
                if (abs(pos[i][0]) != abs(pos[j][0])) {
                    int ddx = x + pos[i][0] + pos[j][0];
                    int ddy = y + pos[i][1] + pos[j][1];
                    if (map[ddx][ddy] != plant) {
                        result[1] += 1;
                    }
                }
            }
        }

    } else if (adjacent == 4) {
        int diagonals[4][2] = {{-1,1}, {1,1}, {1,-1}, {-1,-1}};
        for (int i = 0; i < 4; i++) {
            int dx = diagonals[i][0], dy = diagonals[i][1];
            int ddx = x + dx, ddy = y + dy;
            if (map[ddx][ddy] != plant) {
                result[1]++;
            }
        }
    }
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    char reader[200];
    int size = 140; // 10 for test, 140 for input
    char** input_arr = (char**)malloc(sizeof(char*) * size);
    for (int i = 0; i < size; i++) {
        input_arr[i] = (char*)malloc(sizeof(char) * size);
    }
    bool** seen = (bool**)malloc(sizeof(bool*) * size);
    for (int i = 0; i < size; i++) {
        seen[i] = (bool*)calloc(size, sizeof(bool));
    }

    int row = 0;

    while(fgets(reader, 200, fptr)) {
        for (int col = 0; col < size; col++) {
            input_arr[row][col] = reader[col];
        }
        row++;
    }

    int total_price = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (seen[i][j]) {
                continue;
            }
            int result[] = {0,0};
            dfs(input_arr, seen, size, input_arr[i][j], i, j, result);
            total_price += result[0] * result[1];
        }
    }

    for (int i = 0; i < size; i++) {
        memset(seen[i], 0, size * sizeof(bool));
    }

    int discounted_price = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (seen[i][j]) {
                continue;
            }
            int result[] = {0,0};
            dfs_2(input_arr, seen, size, input_arr[i][j], i, j, result);
            discounted_price += result[0] * result[1];
        }
    }
    
    printf("Total price: %d\n", total_price);
    printf("Discounted price: %d\n", discounted_price);

    for (int i = 0; i < size; i++) {
        free(input_arr[i]);
        free(seen[i]);
    }
    free(input_arr);
    free(seen);
    fclose(fptr);
    return 0;
}