#include "../shared/common.h"

void print_arr_2d(int** arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("[ ");
            for (int j = 0; j < size; j++) {
                printf(" %d ", arr[i][j]);
            }
        printf("]\n");
    }
}

int dfs(int** arr, int size, int x, int y, bool* reached) {
    int directions[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    if (arr[x][y] == 9) {
        if (reached != NULL) {
            if (!reached[x * size + y]) {
                reached[x * size + y] = true;
                return 1;
            }
            return 0;
        }
        return 1;
    }

    int score = 0;
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0], dy = directions[i][1];
        int ddx = x + dx, ddy = y + dy;
        if (ddx >= 0 && ddx < size && ddy >= 0 && ddy < size && arr[ddx][ddy] == arr[x][y] + 1) {
            score += dfs(arr, size, ddx, ddy, reached);
        }
    }
    return score;
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    char reader[50000];
    int size = 40; // 8 for test, 40 for input
    int** map = (int**)malloc(sizeof(int*) * size);
    for (int i = 0; i < size; i++) {
        map[i] = (int*)malloc(sizeof(int) * size);
    }
    
    int row = 0;
    while(fgets(reader, 50000, fptr)) {
        int col = 0;
        while (col < size) {
            map[row][col] = reader[col] - '0';
            col++;
        }
        row++;
    }


    int trailheads = 0, ratings = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (map[i][j] == 0) {
                bool* reached = (bool*)calloc(2000, sizeof(bool));
                trailheads += dfs(map, size, i, j, reached);
                ratings += dfs(map, size, i, j, NULL);
                free(reached);
            }
        }
    }

    fclose(fptr);
    for (int i = 0; i < size; i++) {
        free(map[i]);
    }
    free(map);
    printf("Sum of trailheads scores: %d\n", trailheads);
    printf("Sum of ratings: %d\n", ratings);
    return 0;
}