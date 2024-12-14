#include "../shared/common.h"
#include "../data-structures/linkedlist.h"
#include "../data-structures/heap.h"

void print_arr(int*** arr, int size) {
    printf("[ ");
    for (int i = 0; i < size; i++) {
        printf("[");
        for (int j = 0; j < 2; j++) {
            printf(" [ %d %d ] ", arr[i][j][0], arr[i][j][1]);
        }
        printf("] ");
    }
    printf("]\n");
}

void print_arr_2d(int** arr, int width, int height) {
    for (int i = 0; i < height; i++) {
        printf("[ ");
            for (int j = 0; j < width; j++) {
                printf(" %d ", arr[i][j]);
            }
        printf("]\n");
    }
}

int*** get_robots(FILE *fptr, int size) {
    char reader[50];
    int*** robots = (int***)malloc(sizeof(int**) * size);
    for (int i = 0; i < size; i++) {
        robots[i] = (int**)malloc(sizeof(int*) * 2);
        for (int j = 0; j < 2; j++) {
            robots[i][j] = (int*)calloc(2, sizeof(int));
        }
    }

    for (int i = 0; i < size; i++) {
        fgets(reader, 50, fptr);
        int k = 0;
        for (int j = 0; j < 2; j++) {
            while (reader[k] != '=')  {
                k++;
            }
            k++;
            int tmp = 0;
            bool negative = false;
            while (reader[k] != ',') {
                if (reader[k] == '-') {
                    negative = true;
                } else {
                    tmp *= 10;
                    if (negative) {
                        tmp -= reader[k] - '0';
                    } else {
                        tmp += reader[k] - '0';
                    }
                }
                k++;
            }
            robots[i][j][0] = tmp;
            tmp = 0;
            negative = false;
            k++;
            while (reader[k] != ' ' && reader[k] != '\0' && reader[k] != '\n') {
                if (reader[k] == '-') {
                    negative = true;
                } else {
                    tmp *= 10;
                    if (negative) {
                        tmp -= reader[k] - '0';
                    } else {
                        tmp += reader[k] - '0';
                    }
                }
                k++;
            }
            robots[i][j][1] = tmp;
        }
    }

    return robots;
}

void free_robots(int*** robots, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 2; j++) {
            free(robots[i][j]);
        }
        free(robots[i]);
    }
    free(robots);
}

int** get_map(int width, int height) {
    int** map = (int**)malloc(sizeof(int*) * height);
    for (int i = 0; i < height; i++) {
        map[i] = (int*)calloc(width, sizeof(int));
    }
    return map;
}

void free_map(int** map, int height) {
    for (int i = 0; i < height; i++) {
        free(map[i]);
    }
    free(map);
}

void move_robot(int*** robots, int robot_num, int** map, int width, int height, int seconds) {
    long long x = (long long) robots[robot_num][0][0];
    long long y = (long long) robots[robot_num][0][1];
    long long dx = (long long) robots[robot_num][1][0];
    long long dy = (long long) robots[robot_num][1][1];

    long long ddx = x + seconds * dx;
    long long ddy = y + seconds * dy;

    if (ddx < 0) {
        ddx = ddx + (-1 * ddx / width + 1) * width;
    }
    if (ddy < 0) {
        ddy = ddy + (-1 * ddy / height + 1) * height;
    }

    map[ddy % height][ddx % width] += 1;
}

void move_robot_2(int*** robots, int robot_num, int** map, int width, int height, int *new_x, int *new_y) {
    long long x = (long long) robots[robot_num][0][0];
    long long y = (long long) robots[robot_num][0][1];
    long long dx = (long long) robots[robot_num][1][0];
    long long dy = (long long) robots[robot_num][1][1];

    long long ddx = x + dx;
    long long ddy = y + dy;

    if (ddx < 0) {
        ddx = ddx + (-1 * ddx / width + 1) * width;
    }
    if (ddy < 0) {
        ddy = ddy + (-1 * ddy / height + 1) * height;
    }

    map[ddy % height][ddx % width] += 1;
    map[y][x] -= 1;
    *new_x = ddx % width;
    *new_y = ddy % height;
}

unsigned long long calculate_safety_factor(int** map, int width, int height) {
    int quadrants[4] = {0, 0, 0, 0};
    int middle_x = (width - 1) / 2, middle_y = (height - 1) / 2;

    for (int i = 0; i < height; i++) {
        if (i == middle_y) {
            continue;
        }
        for (int j = 0; j < width; j++) {
            if (j == middle_x) {
                continue;
            }
            if (j < middle_x && i < middle_y) {
                quadrants[0] += map[i][j];
            } else if (j > middle_x &&  i < middle_y) {
                quadrants[1] += map[i][j];
            } else if (j < middle_x && i > middle_y) {
                quadrants[2] += map[i][j];
            } else if (j > middle_x && i > middle_y) {
                quadrants[3] += map[i][j];
            }
        }
    }

    unsigned long long result = (long long) quadrants[0];
    for (int i = 1; i < 4; i++) {
        result *= quadrants[i];
    }
    return result;
}

void reset_map(int** map, int width, int height) {
    for (int i = 0; i < height; i++) {
        memset(map[i], 0, width * sizeof(int));
    }
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    int size = 500; // 12 for test, 500 for input
    int width = 101, height = 103; // 11w 7h for test, 101w 103h for input
    int*** robots = get_robots(fptr, size);
    fclose(fptr);
    int** map = get_map(width, height);

    int seconds = 100;

    for (int i = 0; i < size; i++) {
        move_robot(robots, i, map, width, height, seconds);
    }

    unsigned long long safety_factor = calculate_safety_factor(map, width, height);

    printf("Total safety factor: %llu\n", safety_factor);

    reset_map(map, width, height);

    int min_time = 0;

    for (int i = 0; i < 30000; i++) {
        double avg_x = 0;
        double avg_y = 0;
        for (int j = 0; j < size; j++) {
            int new_x, new_y;
            move_robot_2(robots, j, map, width, height, &new_x, &new_y);
            robots[j][0][0] = new_x;
            robots[j][0][1] = new_y;
            avg_x += new_x;
            avg_y += new_y;
        }
        avg_x /= size;
        avg_y /= size;
        double var_x = 0;
        double var_y = 0;
        for (int j = 0; j < size; j++) {
            var_x += (robots[j][0][0] - avg_x) * (robots[j][0][0] - avg_x);
            var_y += (robots[j][0][1] - avg_y) * (robots[j][0][1] - avg_y);
        }
        var_x /= size;
        var_y /= size;
        min_time = i + 1;
        if (var_x < 650 && var_y < 650) break;
    }

    printf("Minimum time for tree to appear: %d\n", min_time);

    free_map(map, height);
    free_robots(robots, size);
    return 0;
}