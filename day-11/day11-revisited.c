#include "../shared/common.h"

#define CAPACITY 10000

typedef struct HashEntry {
    long long key;
    long long value;
    struct HashEntry* next; // Pointer to the next HashEntry in the bucket (for collision resolution)
} HashEntry;


long long hash_function(long long key) {
    const unsigned long PRIME = 31622787;
    return (long long)((key % PRIME) % CAPACITY);
}

HashEntry* get(HashEntry** hashmap, long long key) {
    long long index = hash_function(key);
    HashEntry* entry = hashmap[index];  // Start at the head of the linked list in the bucket
    while (entry != NULL) {
        if (entry->key == key) {
            return entry;
        }
        entry = entry->next;  // Move to the next entry in the list
    }
    return NULL;  // If no entry is found
}

void insert(HashEntry** hashmap, HashEntry* entry) {
    long long index = hash_function(entry->key);  // Find the correct bucket
    entry->next = hashmap[index];  // Insert at the beginning of the list (chain)
    hashmap[index] = entry;  // Update the bucket to point to the new entry
}

void split_value(long long val, long long* left, long long* right) {
    int digits = 0;
    long long temp = val;
    while (temp > 0) {
        digits++;
        temp /= 10;
    }
    
    // Split the value by position of the digits
    long long divisor = 1;
    for (int i = 0; i < digits / 2; i++) {
        divisor *= 10;
    }

    *left = val / divisor;
    *right = val % divisor;
}

void clear_hash_map(HashEntry** hashmap) {
    for (int i = 0; i < CAPACITY; i++) {
        HashEntry* entry = hashmap[i];
        while (entry != NULL) {
            HashEntry* temp = entry;
            entry = entry->next;
            free(temp);
        }
        hashmap[i] = NULL;
    }
}

void copy_hash_map(HashEntry** src, HashEntry** dst) {
    for (int i = 0; i < CAPACITY; i++) {
        HashEntry* entry = src[i];
        HashEntry* prev = NULL;
        while (entry != NULL) {
            HashEntry* copy = (HashEntry*)malloc(sizeof(HashEntry));
            copy->key = entry->key;
            copy->value = entry->value;
            copy->next = NULL;
            
            if (prev == NULL) {
                dst[i] = copy;
            } else {
                prev->next = copy;
            }
            prev = copy;
            entry = entry->next;
        }
    }
}

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    char reader[50];

    fgets(reader, 50, fptr);

    int i = 0, tmp = 0;
    HashEntry** stones = (HashEntry**)calloc(CAPACITY, sizeof(HashEntry*));

    while(reader[i] != '\0') {
        if (reader[i] == ' ') {
            HashEntry* entry = (HashEntry*)malloc(sizeof(HashEntry));
            entry->key = tmp;
            entry->value = 1;
            insert(stones, entry);
            tmp = 0;
        } else {
            tmp *= 10;
            tmp += reader[i] - '0';
        }
        i++;
    }

    int blinks = 75;
    HashEntry** new_stones = (HashEntry**)calloc(CAPACITY, sizeof(HashEntry*));
    for (int i = 0; i < blinks; i++) {
        for (int j = 0; j < CAPACITY; j++) {
            HashEntry* entry = stones[j];
            while (entry != NULL) {
                long long num = entry->key;
                long long count = entry->value;
                int digits = 0;
                do {
                    num /= 10;
                    digits++;
                } while (num >= 1);
                num = entry->key;
                if (digits % 2 == 1) {
                    long long val = 1;
                    if (num != 0) {
                        val = num * 2024;
                    }
                    HashEntry* found = get(new_stones, val);
                    if (found != NULL) {
                        found->value += count;
                    } else {
                        HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
                        new_entry->key = val;
                        new_entry->value = count;
                        insert(new_stones, new_entry);
                    }
                } else {
                    long long left = 0, right = 0;
                    split_value(num, &left, &right);
                    HashEntry* found = get(new_stones, left);
                    if (found != NULL) {
                        found->value += count;
                    } else {
                        HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
                        new_entry->key = left;
                        new_entry->value = count;
                        insert(new_stones, new_entry);
                    }
                    found = get(new_stones, right);
                    if (found != NULL) {
                        found->value += count;
                    } else {
                        HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
                        new_entry->key = right;
                        new_entry->value = count;
                        insert(new_stones, new_entry);
                    }
                }
                entry = entry->next;
            }
        }
        clear_hash_map(stones);
        copy_hash_map(new_stones, stones);
        clear_hash_map(new_stones);
    }
    free(new_stones);

    long long num_stones = 0;
    for (int i = 0; i < CAPACITY; i++) {
        HashEntry* entry = stones[i];
        while (entry != NULL) {
            num_stones += entry->value;
            entry = entry->next;
        }
    }
    clear_hash_map(stones);
    free(stones);

    printf("Number of stones: %lld\n", num_stones);
    fclose(fptr);
    return 0;
}