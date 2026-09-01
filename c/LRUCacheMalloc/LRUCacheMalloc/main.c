//
//  main.c
//  LRUCacheMalloc
//
//  Created by Anussha on 01/09/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 16

typedef struct Node {
    char key[32];
    int value;
    struct Node* prev;
    struct Node* next;
    struct Node* hashNext; // for hash table chaining (separate from LRU chain)
} Node;

typedef struct {
    int capacity;
    int count;
    Node* head; // most recently used
    Node* tail; // least recently used
    Node* table[TABLE_SIZE]; // hash table buckets, chained via hashNext
} LRUCache;

LRUCache* createCache(int capacity) {
    LRUCache* cache = malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->count = 0;
    cache->head = NULL;
    cache->tail = NULL;
    for (int i = 0; i < TABLE_SIZE; i++) {
        cache->table[i] = NULL;
    }
    return cache;
}

unsigned int hash(const char* key) {
    unsigned int h = 0;
    while (*key) {
        h = h * 31 + (unsigned char)(*key);
        key++;
    }
    return h % TABLE_SIZE;
}

Node* findNode(LRUCache* cache, const char* key) {
    unsigned int idx = hash(key);
    Node* current = cache->table[idx];
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current;
        }
        current = current->hashNext;
    }
    return NULL;
}

void removeFromList(LRUCache* cache, Node* node) {
    if (node->prev) node->prev->next = node->next;
    else cache->head = node->next;

    if (node->next) node->next->prev = node->prev;
    else cache->tail = node->prev;

    node->prev = NULL;
    node->next = NULL;
}

void insertAtHead(LRUCache* cache, Node* node) {
    node->next = cache->head;
    node->prev = NULL;
    if (cache->head) cache->head->prev = node;
    cache->head = node;

    if (!cache->tail) cache->tail = node;
}

void hashInsert(LRUCache* cache, Node* node) {
    unsigned int idx = hash(node->key);
    node->hashNext = cache->table[idx];
    cache->table[idx] = node;
}

void hashRemove(LRUCache* cache, Node* key_owner) {
    unsigned int idx = hash(key_owner->key);
    Node* current = cache->table[idx];
    Node* prevChain = NULL;

    while (current) {
        if (current == key_owner) {
            if (prevChain) prevChain->hashNext = current->hashNext;
            else cache->table[idx] = current->hashNext;
            return;
        }
        prevChain = current;
        current = current->hashNext;
    }
}

int get(LRUCache* cache, const char* key, int* outValue) {
    Node* node = findNode(cache, key);
    if (!node) return 0;

    removeFromList(cache, node);
    insertAtHead(cache, node);

    *outValue = node->value;
    return 1;
}

void put(LRUCache* cache, const char* key, int value) {
    Node* existing = findNode(cache, key);
    if (existing) {
        existing->value = value;
        removeFromList(cache, existing);
        insertAtHead(cache, existing);
        return;
    }

    Node* node = malloc(sizeof(Node));
    strncpy(node->key, key, sizeof(node->key) - 1);
    node->key[sizeof(node->key) - 1] = '\0';
    node->value = value;
    node->prev = NULL;
    node->next = NULL;
    node->hashNext = NULL;

    hashInsert(cache, node);
    insertAtHead(cache, node);
    cache->count++;

    if (cache->count > cache->capacity) {
        Node* lru = cache->tail;
        removeFromList(cache, lru);
        hashRemove(cache, lru);
        free(lru);
        cache->count--;
    }
}

int main(void) {
    LRUCache* cache = createCache(2);

    put(cache, "A", 1);
    put(cache, "B", 2);

    int val;
    int found = get(cache, "A", &val);
    printf("get(A) mid-trace: %s\n", found ? "1" : "missing"); // expect 1

    put(cache, "C", 3);

    found = get(cache, "B", &val);
    printf("get(B) after eviction: %s\n", found ? "found" : "missing"); // expect missing

    found = get(cache, "A", &val);
    printf("get(A) still present: %d\n", found ? val : -1); // expect 1

    found = get(cache, "C", &val);
    printf("get(C) still present: %d\n", found ? val : -1); // expect 3

    // Cleanup
    Node* current = cache->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(cache);

    printf("Cache freed cleanly\n");

    return 0;
}
