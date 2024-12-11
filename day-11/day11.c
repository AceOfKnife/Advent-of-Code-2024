#include "../shared/common.h"
#include "../data-structures/linkedlist.h"

#define CAPACITY 100000000

typedef struct HashEntry {
    long long key;
    long long left;
    long long right;
    bool split;
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

int main() {
    FILE *fptr = fopen("./input.txt", "r");
    char reader[50];

    fgets(reader, 50, fptr);

    int i = 0, tmp = 0;
    LinkedList* linkedList = createLinkedList();

    while(reader[i] != '\0') {
        if (reader[i] == ' ') {
            pushRight(linkedList, tmp);
            tmp = 0;
        } else {
            tmp *= 10;
            tmp += reader[i] - '0';
        }
        i++;
    }

    int blinks = 25;
    HashEntry** hashmap = (HashEntry**)malloc(sizeof(HashEntry*) * CAPACITY);

    for (int i = 0; i < blinks; i++) {
        printf("Run %d...\n", i+1);
        Node* cur_node = linkedList->head->next;
        while (cur_node != linkedList->tail) {
            Node* next_node = cur_node->next;
            long long val = cur_node->val;
            HashEntry* result = get(hashmap, val);
            if (result != NULL) {
                if (!result->split) {
                    Node* node = (Node*)malloc(sizeof(Node));
                    node->val = result->left;
                    node->next = cur_node->next;
                    node->prev = cur_node->prev;

                    cur_node->prev->next = node;
                    cur_node->next->prev = node;

                    free(cur_node);
                    cur_node = next_node;
                } else {
                    Node* left_node = (Node*)malloc(sizeof(Node));
                    Node* right_node = (Node*)malloc(sizeof(Node));
                    left_node->val = result->left;
                    right_node->val =  result->right;

                    left_node->prev = cur_node->prev;
                    left_node->next = right_node;
                    right_node->prev = left_node;
                    right_node->next = cur_node->next;

                    cur_node->prev->next = left_node;
                    cur_node->next->prev = right_node;

                    free(cur_node); 
                    cur_node = next_node; 
                    linkedList->size++;
                }
            } else {
                long long tmp = val;
                int digits = 0;
                do {
                    tmp /= 10;
                    digits++;
                } while (tmp >= 1);
                HashEntry* entry = (HashEntry*)malloc(sizeof(HashEntry));
                entry->key = val;
                if (digits == 0 || digits % 2 == 1) {
                    if (val == 0) {
                        val = 1;
                    } else {
                        val *= 2024;
                    }

                    entry->left = val;
                    entry->split = false;
                    insert(hashmap, entry);

                    Node* node = (Node*)malloc(sizeof(Node));
                    node->val = val;
                    node->next = cur_node->next;
                    node->prev = cur_node->prev;

                    cur_node->prev->next = node;
                    cur_node->next->prev = node;

                    free(cur_node);
                    cur_node = next_node;
                } else {
                    long long left = 0, right = 0;
                    split_value(val, &left, &right);
                    
                    entry->left = left;
                    entry->right = right;
                    entry->split = true;
                    insert(hashmap, entry);

                    Node* left_node = (Node*)malloc(sizeof(Node));
                    Node* right_node = (Node*)malloc(sizeof(Node));
                    left_node->val = left;
                    right_node->val = right;

                    left_node->prev = cur_node->prev;
                    left_node->next = right_node;
                    right_node->prev = left_node;
                    right_node->next = cur_node->next;

                    cur_node->prev->next = left_node;
                    cur_node->next->prev = right_node;

                    free(cur_node); 
                    cur_node = next_node; 
                    linkedList->size++;
                }
            }
        }
    }
    printf("Number of stones: %d\n", getSize(linkedList));
    freeList(linkedList);
    for (int i = 0; i < CAPACITY; i++) {
        HashEntry* entry = hashmap[i];
        while (entry != NULL) {
            HashEntry* temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(hashmap);

    fclose(fptr);
    return 0;
}