//
//  main.c
//  MemoryPool
//
//  Created by Anussha on 26/08/26.
//
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE sizeof(void*)   // each block must be at least pointer-sized
#define POOL_BLOCKS 8

typedef struct FreeNode{
    struct FreeNode *next;
} FreeNode;

typedef struct {
    char *pool ; // raw memory backing the whole pool
    FreeNode *freeList; // head of the free block list
    size_t blockSize;
    int totalBlocks;
}memoryPool;

void poolInit(memoryPool *mp,size_t blockSize,int numBlocks){
    if(blockSize < sizeof(FreeNode)){
        blockSize = sizeof(FreeNode); // block must fit a FreeNode pointer
    }
    mp->blockSize = blockSize;
    mp->totalBlocks = numBlocks;
    mp->pool = malloc(blockSize * numBlocks);
    
    // Thread the free list through the raw memory: each block's first
    // bytes become a FreeNode pointing to the next block.
    
    mp->freeList = (FreeNode *)mp->pool;
    FreeNode *current = mp->freeList;
    for (int i = 0; i < numBlocks-1; i++) {
        current->next = (FreeNode *)(mp->pool + (i+1) * blockSize);
        current = current->next;
    }
    current->next = NULL;  // last block terminates the list
    printf("Pool initialized: %d blocks of %zu bytes\n", numBlocks, blockSize);
}

void *poolAlloc(memoryPool *mp) {
    if (mp->freeList == NULL) {
        printf("Pool exhausted!\n");
        return NULL;
    }
    FreeNode *block = mp->freeList;
    mp->freeList = block->next;   // pop the head off the free list
    printf("Allocated block at %p\n", (void*)block);
    return (void *)block;
}

void poolFree(memoryPool *mp, void *block) {
    FreeNode *node = (FreeNode *)block;
    node->next = mp->freeList;   // push it back onto the free list
    mp->freeList = node;
    printf("Freed block at %p\n", block);
}
void poolDestroy(memoryPool *mp) {
    free(mp->pool);
    mp->pool = NULL;
    mp->freeList = NULL;
    printf("Pool destroyed\n");
}

/*int main(void) {
    memoryPool mp;
    poolInit(&mp, BLOCK_SIZE, POOL_BLOCKS);

    void *a = poolAlloc(&mp);
    void *b = poolAlloc(&mp);
    void *c = poolAlloc(&mp);

    poolFree(&mp, b);

    void *d = poolAlloc(&mp);  // should reuse b's address, since b was freed last

    return 0;
}
*/

int main(void) {
    memoryPool mp;
    poolInit(&mp, BLOCK_SIZE, POOL_BLOCKS);

    void *blocks[POOL_BLOCKS];
    for (int i = 0; i < POOL_BLOCKS; i++) {
        blocks[i] = poolAlloc(&mp);
    }

    // Pool should be exhausted now - this call should fail gracefully
    void *extra = poolAlloc(&mp);
    if (extra == NULL) {
        printf("Correctly returned NULL when pool exhausted\n");
    }
    poolDestroy(&mp);
    return 0;
}
