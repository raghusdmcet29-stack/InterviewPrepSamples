//
//  main.c
//  MemoryPoolDemo
//
//  Created by Anussha on 27/08/26.
//
#include <stdio.h>
#include <stdlib.h>

#define POOL_SIZE 5
#define BLOCK_SIZE sizeof(int)

typedef struct {
    char memory[POOL_SIZE * BLOCK_SIZE];
    int used[POOL_SIZE];
}MemoryPool;

void pool_init(MemoryPool* pool){
    for (int i =0; i<POOL_SIZE; i++) {
        pool->used[i] = 0;
    }
}

void * pool_alloc(MemoryPool* pool){
    for (int i = 0; i<POOL_SIZE; i++) {
        if (!pool->used[i]) {
            pool->used[i] = 1;
            return &pool->memory[i * BLOCK_SIZE];
        }
    }
    return NULL; // pool exhausted
}

void pool_free(MemoryPool* pool,void* ptr){
    size_t offset = (char*)ptr - pool->memory;
    int index = (int) offset / BLOCK_SIZE;
    pool->used[index] = 0;
}

int main(void){
    MemoryPool pool;
    pool_init(&pool);
    
    int* a = (int*)pool_alloc(&pool);
    int* b = (int*)pool_alloc(&pool);
    *a = 100;
    *b = 200;
    printf("a = %d, b = %d\n", *a, *b);
    pool_free(&pool, a);
    int* c = (int*)pool_alloc(&pool); // should reuse a's slot
    *c = 300;
    printf("c (reused slot) = %d\n", *c);
    
    // exhaust the pool
        void* ptrs[POOL_SIZE];
        int count = 0;
        for (int i = 0; i < POOL_SIZE; i++) {
            void* p = pool_alloc(&pool);
            if (p == NULL) break;
            ptrs[count++] = p;
        }
        printf("Allocated %d more before exhaustion\n", count);
    void* overflow = pool_alloc(&pool);
    printf("Allocation when full: %s\n", overflow == NULL ? "NULL (correctly refused)" : "unexpected non-NULL");

    return 0;
}
