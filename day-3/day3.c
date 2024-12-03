#include "../shared/common.h"

void reset_vars(int* i, int* val_1, int* val_2, int* tmp, char c) {
    *i = (c == 'm') ? 1 : 0, *val_1 = -1, *val_2 = -1, *tmp = -1;
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    char reader[5000];

    int mul_sum = 0;
    bool mul_enabled = true;

    while (fgets(reader, 5000, fptr)) {
        char c = reader[0], mul[4] = {'m', 'u', 'l', '('};
        char con_1[4] = {'d', 'o', '(', ')'}, con_2[7] = {'d','o','n','\'','t','(',')'};
        int j = 0, k = 0;
        int idx = 0, i = 0, val_1 = -1, val_2 = -1, tmp = -1;
        while (c != '\0') {

            if (j < 4 && c == con_1[j]) {
                j++;
            } else if (j >= 4) {
                mul_enabled = true;
                j = 0;
            } else {
                j = c == 'd';
            }

            if (k < 7 && c == con_2[k]) {
                k++;
            } else if (k >= 7) {
                mul_enabled = false;
                k = 0;
            } else {
                k = c == 'd';
            }

            if (i < 4 && c == mul[i]) {
                i++;
            } else if (i >= 4) {
                if (c == ',' && val_1 == -1 && tmp != -1) {
                    val_1 = tmp;
                    tmp = -1;
                } else if (c == ')' && val_2 == -1 && tmp != -1) {
                    val_2 = tmp;
                    mul_sum += (mul_enabled) ? val_1 * val_2 : 0;
                    reset_vars(&i, &val_1, &val_2, &tmp, c);
                } else if (isdigit(c)) {
                    tmp = (tmp == -1) ? 0 : tmp;
                    tmp *= 10;
                    tmp += c - '0';
                } else {
                    reset_vars(&i, &val_1, &val_2, &tmp, c);
                }
            } else {
                reset_vars(&i, &val_1, &val_2, &tmp, c);
            }
            idx++;
            c = reader[idx];
        }
    }
    fclose(fptr);
    printf("Uncorrupted mul sum: %d\n", mul_sum);
    return 0;
}