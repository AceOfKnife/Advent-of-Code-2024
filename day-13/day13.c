#include "../shared/common.h"
#include "../data-structures/linkedlist.h"

void print_arr(int*** arr, int size) {
    printf("[ ");
    for (int i = 0; i < size; i++) {
        printf("[");
        for (int j = 0; j < 3; j++) {
            printf(" [ %d %d ] ", arr[i][j][0], arr[i][j][1]);
        }
        printf("] ");
    }
    printf("]\n");
}

void print_matrix(double** arr) {
    for (int i = 0; i < 2; i++) {
        printf("[ ");
            for (int j = 0; j < 2; j++) {
                printf(" %lf ", arr[i][j]);
            }
        printf("]\n");
    }
}
int*** get_machines(FILE *fptr, int size) {
    char reader[50];
    int*** machines = (int***)malloc(sizeof(int**) * size);
    for (int i = 0; i < size; i++) {
        machines[i] = (int**)malloc(sizeof(int*) * 3);
        for (int j = 0; j < 3; j++) {
            machines[i][j] = (int*)calloc(2, sizeof(int));
        }
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 3; j++) {
            fgets(reader, 50, fptr);
            int k = 0, tmp = 0;
            while (reader[k] != '\0' && reader[k] != '+' && reader[k] !='=') {
                k++;
            }
            k++;
            while (reader[k] != '\0' && reader[k] != ',') {
                tmp *= 10;
                tmp += reader[k] - '0';
                k++;
            }
            machines[i][j][0] = tmp;
            tmp = 0;
            while (reader[k] != '\0' && reader[k] != '+' && reader[k] !='=') {
                k++;
            }
            k++;
            while (reader[k] != '\0' && reader[k] !='\n') {
                tmp *= 10;
                tmp += reader[k] - '0';
                k++;
            }
            machines[i][j][1] = tmp;
        }
        fgets(reader, 50, fptr);
    }

    return machines;
}

void free_machines(int*** machines, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 3; j++) {
            free(machines[i][j]);
        }
        free(machines[i]);
    }
    free(machines);
}

int bfs_tokens(int*** machines, int size) {
    int total_tokens = 0;

    for (int i = 0; i < size; i++) {
        bool* cache = (bool*)calloc(15000, sizeof(bool));
        int min_tokens = -1;
        int a_dx = machines[i][0][0], a_dy = machines[i][0][1];
        int b_dx = machines[i][1][0], b_dy = machines[i][1][1];
        int target_x = machines[i][2][0], target_y = machines[i][2][1];
        LinkedList* deque = createLinkedList();
        Node* start = (Node*)malloc(sizeof(Node));
        start->a = 0;
        start->b = 0;
        pushRightNode(deque, start);
        while (getSize(deque)) {
            Node* claw = popLeftNode(deque);
            int new_x = claw->a * a_dx + claw->b * b_dx;
            int new_y = claw->a * a_dy + claw->b * b_dy;
            int cost = claw->a * 3 + claw->b;
            if (new_x == target_x && new_y == target_y) {
                if (min_tokens == -1) {
                    min_tokens = cost;
                } else {
                    min_tokens = (min_tokens < cost) ? min_tokens : cost;
                }
                free(claw);
                continue;
            }

            if (claw->a < 100) {
                int key = 101 * (claw->a + 1) + (claw->b);
                if (!cache[key]) {
                    cache[key] = true;
                    Node* new_claw = (Node*)malloc(sizeof(Node));
                    new_claw->a = claw->a + 1;
                    new_claw->b = claw->b;
                    pushRightNode(deque, new_claw);
                }
            }

            if (claw->b < 100) {
                int key = 101 * (claw->a) + (claw->b + 1);
                if (!cache[key]) {
                    cache[key] = true;
                    Node* new_claw = (Node*)malloc(sizeof(Node));
                    new_claw->a = claw->a;
                    new_claw->b = claw->b + 1;
                    pushRightNode(deque, new_claw);
                }
            }
            free(claw);
        }
        total_tokens += (min_tokens != -1) ? min_tokens : 0;
        freeList(deque);
        free(cache);
    }

    return total_tokens;
}

double** invert_matrix(double** matrix) {
    double det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    if (det == 0) {
        return NULL;
    }
    double** new_matrix = (double**)malloc(sizeof(double*) * 2);
    for (int i = 0; i < 2; i++) {
        new_matrix[i] = (double*)malloc(sizeof(double) * 2);
    }
    new_matrix[0][0] = matrix[1][1];
    new_matrix[1][1] = matrix[0][0];
    new_matrix[0][1] = -1 * matrix[0][1];
    new_matrix[1][0] = -1 * matrix[1][0];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            new_matrix[i][j] /= det;
        }
    }
    return new_matrix;
}

double* mat_mul(double** matrix, double* vector) {
    double* result = (double*)malloc(sizeof(double*) * 2);
    for (int i = 0; i < 2; i++) {
        result[i] = vector[0] * matrix[i][0] + vector[1] * matrix[i][1];
    }
    return result;
}

bool tolerance(long long target_x, long long target_y, int mode, double* result, double** matrix) {
    if (mode == 0) {
        bool ans = (long long)(floor(result[0]) * matrix[0][0] + floor(result[1]) * matrix[0][1]) == target_x &&
                   (long long)(floor(result[0]) * matrix[1][0] + floor(result[1]) * matrix[1][1]) == target_y;
        if (ans) {
            result[0] = floor(result[0]);
            result[1] = floor(result[1]);
        }
        return ans;
    } else if (mode == 1) {
        
        bool ans = (long long)(ceil(result[0]) * matrix[0][0] + floor(result[1]) * matrix[0][1]) == target_x &&
                   (long long)(ceil(result[0]) * matrix[1][0] + floor(result[1]) * matrix[1][1]) == target_y;
        if (ans) {
            result[0] = ceil(result[0]);
            result[1] = floor(result[1]);
        }
        return ans;
    } else if (mode == 2) {
        bool ans = (long long)(floor(result[0]) * matrix[0][0] + ceil(result[1]) * matrix[0][1]) == target_x &&
                   (long long)(floor(result[0]) * matrix[1][0] + ceil(result[1]) * matrix[1][1]) == target_y;
        if (ans) {
            result[0] = floor(result[0]);
            result[1] = ceil(result[1]);
        }
        return ans;
    } else {
        bool ans = (long long)(ceil(result[0]) * matrix[0][0] + ceil(result[1]) * matrix[0][1]) == target_x &&
                   (long long)(ceil(result[0]) * matrix[1][0] + ceil(result[1]) * matrix[1][1]) == target_y;
        if (ans) {
            result[0] = ceil(result[0]);
            result[1] = ceil(result[1]);
        }
        return ans;
    }
}

long long maths_tokens(int*** machines, int size) {
    long long total_tokens = 0;
    for (int i = 0; i < size; i++) {
        double** matrix = (double**)malloc(sizeof(double*) * 2);
        for (int j = 0; j < 2; j++) {
            matrix[j] = (double*)malloc(2 * sizeof(double));
            for (int k = 0; k < 2; k++) {
                matrix[j][k] = machines[i][k][j];
            }
        }
        long long offset = 10000000000000;
        double vector[2] = {machines[i][2][0] + offset, machines[i][2][1] + offset};
        double** inverted = invert_matrix(matrix);
        if (inverted == NULL) {
            continue;
        }
        double* result = mat_mul(inverted, vector);
        
        long long target_x = (long long)vector[0], target_y = (long long)vector[1];

        for (int j = 0; j < 4; j++) {
            if (tolerance(target_x, target_y, j, result, matrix)) {
                total_tokens += 3 * result[0] + result[1];
                break;
            }
        }

        for (int j = 0; j < 2; j++) {
            free(matrix[j]);
        }
        free(matrix);
        free(result);
        free(inverted);
    }
    return total_tokens;
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    int size = 320; // 4 for test, 320 for input
    int*** machines = get_machines(fptr, size);
    fclose(fptr);

    int total_tokens = bfs_tokens(machines, size);
    
    printf("Minimum number of tokens: %d\n", total_tokens);

    long long all_tokens = maths_tokens(machines, size);

    printf("More tokens: %lld\n", all_tokens);

    free_machines(machines, size);

    return 0;
}