#include "../shared/common.h"

void print_arr(int* arr, int size) {
    char moves[4] = {'^', '>', 'v', '<'};
    printf("[");
    for (int i = 0; i < size; i++) {
        printf(" %c ", moves[arr[i]]);
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

char** get_map(FILE *fptr, int size, int* robot_x, int* robot_y) {
    char reader[100];
    char** map = (char**)malloc(sizeof(char*) * size);
    for (int i = 0; i < size; i++) {
        map[i] = (char*)malloc(sizeof(char) * size);
    }

    int row = 0;
    do {
        fgets(reader, 100, fptr);
        int col = 0;
        while (reader[col] != '\n') {
            if (reader[col] == '@') {
                *robot_x = col;
                *robot_y = row;
            }
            map[row][col] = reader[col];
            col++;
        }
        row++;
    } while (reader[0] != '\n');

    return map;
}

void free_map(char** map, int size) {
    for (int i = 0; i < size; i++) {
        free(map[i]);
    }
    free(map);
}

int* get_moves(FILE *fptr, int* moves_size) {
    char reader[2000];
    *moves_size = 0;
    int capacity = 25000; // 1000 for test, 25000 for input
    int* moves = (int*)malloc(sizeof(int) * capacity);

    while (fgets(reader, 100, fptr)) {
        int col = 0;
        while (reader[col] != '\n' && reader[col] != '\0') {
            if (reader[col] == '^') {
                moves[*moves_size] = 0;
            } else if (reader[col] == '>') {
                moves[*moves_size] = 1;
            } else if (reader[col] == 'v') {
                moves[*moves_size] = 2;
            } else if (reader[col] == '<') {
                moves[*moves_size] = 3;
            }
            (*moves_size)++;
            col++;
        }
    }

    return moves;
}

bool can_move(char** map, int map_size, int x, int y, int direction) {
    int directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    if (map[y][x] == '#') {
        return false;
    }

    if (map[y][x] == '.') {
        return true;
    }

    int dx = directions[direction][1], dy = directions[direction][0];
    int ddx = x + dx, ddy = y + dy;

    if (ddx >= 0 && ddx < map_size && ddy >= 0 && ddy < map_size) {
        bool move_obj = can_move(map, map_size, ddx, ddy, direction);
        if (move_obj) {
            map[ddy][ddx] = map[y][x];
            map[y][x] = '.';
            return true;
        }
    }

    return false;

}

void move_robot(char** map, int map_size, int* moves, int moves_size, int robot_x, int robot_y) {
    int directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    for (int i = 0; i < moves_size; i++) {
        int dx = directions[moves[i]][1], dy = directions[moves[i]][0];
        int ddx = robot_x + dx, ddy = robot_y + dy;
        if (ddx >= 0 && ddx < map_size && ddy >= 0 && ddy < map_size) {
            if (map[ddy][ddx] == '.') {
                map[ddy][ddx] = '@';
                map[robot_y][robot_x] = '.';
                robot_x = ddx, robot_y = ddy;
            } else if (map[ddy][ddx] == 'O' && can_move(map, map_size, robot_x, robot_y, moves[i])) {
                robot_x = ddx, robot_y = ddy;
            }
        }
    }
}

char** double_map(char** map, int map_size) {
    int new_map_size = map_size * 2;
    char** new_map = (char**)malloc(sizeof(char*) * map_size);
    for (int i = 0; i < map_size; i++) {
        new_map[i] = (char*)malloc(sizeof(char) * new_map_size);
    }
    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size; j++) {
            if (map[i][j] == '#') {
                new_map[i][j * 2] = '#';
                new_map[i][j * 2 + 1] = '#';
            } else if (map[i][j] == 'O') {
                new_map[i][j * 2] = '[';
                new_map[i][j * 2 + 1] = ']';
            } else if (map[i][j] == '.') {
                new_map[i][j * 2] = '.';
                new_map[i][j * 2 + 1] = '.';
            } else if (map[i][j] == '@') {
                new_map[i][j * 2] = '@';
                new_map[i][j * 2 + 1] = '.';
            }
        }
    }
    return new_map;
}

bool can_move_2(char** map, int map_size, int x, int y, int direction) {
    int directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    if (map[y][x] == '#') {
        return false;
    }

    if (map[y][x] == '.') {
        return true;
    }

    int dx = directions[direction][1], dy = directions[direction][0];
    int ddx = x + dx, ddy = y + dy;

    if (ddx < 0 || ddx >= map_size * 2 || ddy < 0 || ddy >= map_size) {
        return false;
    }

    if ((direction == 0 || direction == 2) && (map[y][x] == '[' || map[y][x] == ']')) {
        int ddx_2 = (map[y][x] == '[') ? x + 1 : x - 1;
        return can_move_2(map, map_size, ddx, ddy, direction) && can_move_2(map, map_size, ddx_2, ddy, direction);
        // if (can_move_2(map, map_size, ddx, ddy, direction) && can_move_2(map, map_size, ddx_2, ddy, direction)) {
        //     map[ddy][ddx] = map[y][x];
        //     map[y][x] = '.';
        //     map[ddy][ddx_2] = map[y][ddx_2 - dx];
        //     map[y][ddx_2 - dx] = '.';
        //     return true;
        // }
    }

    return can_move_2(map, map_size, ddx, ddy, direction);
    // if (can_move_2(map, map_size, ddx, ddy, direction)) {
    //     map[ddy][ddx] = map[y][x];
    //     map[y][x] = '.';
    //     return true;
    // }

}

void move_objects(char** map, int map_size, int x, int y, int direction) {
    int directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    if (map[y][x] == '.') {
        return;
    }

    int dx = directions[direction][1], dy = directions[direction][0];
    int ddx = x + dx, ddy = y + dy;

    if ((direction == 0 || direction == 2) && (map[y][x] == '[' || map[y][x] == ']')) {
        int ddx_2 = (map[y][x] == '[') ? x + 1 : x - 1;
        move_objects(map, map_size, ddx, ddy, direction);
        move_objects(map, map_size, ddx_2, ddy, direction);
        map[ddy][ddx] = map[y][x];
        map[y][x] = '.';
        map[ddy][ddx_2] = map[y][ddx_2 - dx];
        map[y][ddx_2 - dx] = '.';
    } else {
        move_objects(map, map_size, ddx, ddy, direction);
        map[ddy][ddx] = map[y][x];
        map[y][x] = '.';
    }

}

void move_robot_2(char** map, int map_size, int* moves, int moves_size, int robot_x, int robot_y) {
    int directions[4][2] = {{-1,0}, {0,1}, {1,0}, {0,-1}};
    for (int i = 0; i < moves_size; i++) {
        int dx = directions[moves[i]][1], dy = directions[moves[i]][0];
        int ddx = robot_x + dx, ddy = robot_y + dy;
        if (ddx >= 0 && ddx < map_size * 2 && ddy >= 0 && ddy < map_size) {
            if (map[ddy][ddx] == '.') {
                map[ddy][ddx] = '@';
                map[robot_y][robot_x] = '.';
                robot_x = ddx, robot_y = ddy;
            } else if ((map[ddy][ddx] == '[' || map[ddy][ddx] == ']') && can_move_2(map, map_size, robot_x, robot_y, moves[i])) {
                move_objects(map, map_size, robot_x, robot_y, moves[i]);
                robot_x = ddx, robot_y = ddy;
            }
        }
        // print_arr_2d(map, map_size, map_size*2);
        // printf("\n");
    }
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    int map_size = 50; // 10 for test, 50 for input
    int robot_x, robot_y;
    char** map = get_map(fptr, map_size, &robot_x, &robot_y);
    int moves_size;
    int* moves = get_moves(fptr, &moves_size);
    move_robot(map, map_size, moves, moves_size, robot_x, robot_y);
    
    long long gps_sum = 0;
    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size; j++) {
            if (map[i][j] == 'O') {
                gps_sum += 100 * i + j;
            }
        }
    }

    printf("Sum of box GPS coordinates: %lld\n", gps_sum);
    free_map(map, map_size);

    fptr = fopen("./input.txt", "r");
    map = get_map(fptr, map_size, &robot_x, &robot_y);
    robot_x *= 2;
    char** doubled_map = double_map(map, map_size);
    move_robot_2(doubled_map, map_size, moves, moves_size, robot_x, robot_y);

    gps_sum = 0;
    for (int i = 0; i < map_size; i++) {
        for (int j = 0; j < map_size * 2; j++) {
            if (doubled_map[i][j] == '[') {
                gps_sum += 100 * i + j;
            }
        }
    }
    printf("New sum of box GPS coordinates: %lld\n", gps_sum);
    
    fclose(fptr);
    free_map(map, map_size);
    free_map(doubled_map, map_size);
    free(moves);
    return 0;
}