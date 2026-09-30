//
//  main.c
//  MemoryPools
//
//  Created by Anussha on 30/09/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    int capacity;
    int *block;
    int nextFree;
    int *returned;       // NEW
    int returnedCount;   // NEW
} IntPool;

void poolCreate(IntPool *pool, int capacity) {
    pool->capacity = capacity;
    pool->block = malloc(capacity * sizeof(int));
    pool->nextFree = 0;
    pool->returned = malloc(capacity * sizeof(int));   // NEW
    pool->returnedCount = 0;                           // NEW
    printf("Pool created: %d slots\n", capacity);
}

// CHANGED: checks the returned list first
int *poolAllocateSlot(IntPool *pool) {
    if (pool->returnedCount > 0) {
        pool->returnedCount--;
        int index = pool->returned[pool->returnedCount];
        return pool->block + index;
    }
    if (pool->nextFree >= pool->capacity) {
        return NULL;
    }
    int *slot = pool->block + pool->nextFree;
    pool->nextFree++;
    return slot;
}

// NEW
void poolGiveBack(IntPool *pool, int *slot) {
    int index = (int)(slot - pool->block);
    pool->returned[pool->returnedCount] = index;
    pool->returnedCount++;
}

void poolDestroy(IntPool *pool) {
    free(pool->block);
    pool->block = NULL;
    free(pool->returned);      // NEW
    pool->returned = NULL;     // NEW
    printf("Pool freed\n");
}

// CHANGED
int main(void) {
    IntPool pool;
    poolCreate(&pool, 4);

    int *slots[4];
    for (int i = 1; i <= 4; i++) {
        int *slot = poolAllocateSlot(&pool);
        if (slot != NULL) {
            *slot = i * 10;
            slots[i - 1] = slot;
        }
    }

    poolGiveBack(&pool, slots[0]);
    poolGiveBack(&pool, slots[3]);

    for (int i = 1; i <= 3; i++) {
        int *slot = poolAllocateSlot(&pool);
        if (slot != NULL) {
            printf("Request %d: got slot %d\n", i, (int)(slot - pool.block));
        } else {
            printf("Request %d: refused\n", i);
        }
    }

    poolDestroy(&pool);
    return 0;
}

