//
//  main.c
//  Copy-on-Write internals
//
//  Created by Anussha on 29/09/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int refCount;
    int count;
    int *data;
} Storage;

typedef struct {
    Storage *storage;
} IntArray;

IntArray arrayCreate(void) {
    IntArray arr;
    arr.storage = malloc(sizeof(Storage));
    arr.storage->refCount = 1;
    arr.storage->count = 3;
    arr.storage->data = malloc(3 * sizeof(int));
    arr.storage->data[0] = 1;
    arr.storage->data[1] = 2;
    arr.storage->data[2] = 3;
    return arr;
}

IntArray arrayShare(IntArray source) {
    source.storage->refCount++;
    return source;
}

void arrayWrite(IntArray *arr, int index, int value) {
    if (arr->storage->refCount > 1) {
        Storage *old = arr->storage;
        Storage *copy = malloc(sizeof(Storage));
        copy->refCount = 1;
        copy->count = old->count;
        copy->data = malloc(old->count * sizeof(int));
        memcpy(copy->data, old->data, old->count * sizeof(int));
        old->refCount--;
        arr->storage = copy;
    }
    arr->storage->data[index] = value;
}

void arrayRelease(IntArray *arr) {
    arr->storage->refCount--;
    if (arr->storage->refCount == 0) {
        printf("freeing buffer %p\n", (void *)arr->storage->data);
        free(arr->storage->data);
        free(arr->storage);
    }
    arr->storage = NULL;
}

int main(void) {
    IntArray a = arrayCreate();
    IntArray b = arrayShare(a);

    printf("before write, a.data: %p\n", (void *)a.storage->data);
    printf("before write, b.data: %p\n", (void *)b.storage->data);

    arrayWrite(&b, 0, 99);

    printf("after write, a.data: %p\n", (void *)a.storage->data);
    printf("after write, b.data: %p\n", (void *)b.storage->data);
    printf("a.data[0]: %d\n", a.storage->data[0]);
    printf("b.data[0]: %d\n", b.storage->data[0]);

    arrayRelease(&b);
    arrayRelease(&a);
    printf("done\n");
    return 0;
}
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int refCount;
    int count;
    int *data;
} Storage;

typedef struct {
    Storage *storage;
} IntArray;

IntArray arrayCreate(void) {
    IntArray arr;
    arr.storage = malloc(sizeof(Storage));
    arr.storage->refCount = 1;
    arr.storage->count = 3;
    arr.storage->data = malloc(3 * sizeof(int));
    arr.storage->data[0] = 1;
    arr.storage->data[1] = 2;
    arr.storage->data[2] = 3;
    return arr;
}

IntArray arrayShare(IntArray source) {
    source.storage->refCount++;
    return source;
}

void arrayRelease(IntArray *arr) {
    arr->storage->refCount--;
    if (arr->storage->refCount == 0) {
        printf("freeing buffer %p\n", (void *)arr->storage->data);
        free(arr->storage->data);
        free(arr->storage);
    }
    arr->storage = NULL;
}

int main(void) {
    IntArray a = arrayCreate();
    IntArray b = arrayShare(a);
    printf("refCount: %d\n", a.storage->refCount);

    arrayRelease(&b);
    printf("refCount after releasing b: %d\n", a.storage->refCount);

    arrayRelease(&a);
    printf("done\n");
    return 0;
}*/
/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int refCount;
    int count;
    int *data;
} Storage;

typedef struct {
    Storage *storage;
} IntArray;

IntArray arrayCreate(void) {
    IntArray arr;
    arr.storage = malloc(sizeof(Storage));
    arr.storage->refCount = 1;
    arr.storage->count = 3;
    arr.storage->data = malloc(3 * sizeof(int));
    arr.storage->data[0] = 1;
    arr.storage->data[1] = 2;
    arr.storage->data[2] = 3;
    return arr;
}

IntArray arrayShare(IntArray source) {
    source.storage->refCount++;
    return source;
}

void arrayWrite(IntArray *arr, int index, int value) {
    if (arr->storage->refCount > 1) {
        Storage *old = arr->storage;
        Storage *copy = malloc(sizeof(Storage));
        copy->refCount = 1;
        copy->count = old->count;
        copy->data = malloc(old->count * sizeof(int));
        memcpy(copy->data, old->data, old->count * sizeof(int));
        old->refCount--;
        arr->storage = copy;
    }
    arr->storage->data[index] = value;
}

int main(void) {
    IntArray a = arrayCreate();
    IntArray b = arrayShare(a);

    printf("before write, a.data: %p\n", (void *)a.storage->data);
    printf("before write, b.data: %p\n", (void *)b.storage->data);

    arrayWrite(&b, 0, 99);

    printf("after write, a.data: %p\n", (void *)a.storage->data);
    printf("after write, b.data: %p\n", (void *)b.storage->data);
    printf("a.data[0]: %d\n", a.storage->data[0]);
    printf("b.data[0]: %d\n", b.storage->data[0]);
    printf("a refCount: %d\n", a.storage->refCount);
    printf("b refCount: %d\n", b.storage->refCount);

    free(a.storage->data);
    free(a.storage);
    free(b.storage->data);
    free(b.storage);
    return 0;
}*/
/*
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int refCount;
    int count;
    int *data;
} Storage;

typedef struct {
    Storage *storage;
} IntArray;

IntArray arrayCreate(void) {
    IntArray arr;
    arr.storage = malloc(sizeof(Storage));
    arr.storage->refCount = 1;
    arr.storage->count = 3;
    arr.storage->data = malloc(3 * sizeof(int));
    arr.storage->data[0] = 1;
    arr.storage->data[1] = 2;
    arr.storage->data[2] = 3;
    return arr;
}

IntArray arrayShare(IntArray source) {
    source.storage->refCount++;
    return source;
}

int main(void) {
    IntArray a = arrayCreate();
    printf("refCount after create: %d\n", a.storage->refCount);

    IntArray b = arrayShare(a);
    printf("refCount after share: %d\n", a.storage->refCount);

    printf("a.data: %p\n", (void *)a.storage->data);
    printf("b.data: %p\n", (void *)b.storage->data);

    free(a.storage->data);
    free(a.storage);
    return 0;
}*/

/*#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *data;
    int count;
} IntArray;

int main(void) {
    IntArray a;
    a.count = 3;
    a.data = malloc(3 * sizeof(int));
    a.data[0] = 1;
    a.data[1] = 2;
    a.data[2] = 3;

    IntArray b = a;

    printf("a.data: %p\n", (void *)a.data);
    printf("b.data: %p\n", (void *)b.data);

    b.data[0] = 99;

    printf("a.data[0]: %d\n", a.data[0]);
    printf("b.data[0]: %d\n", b.data[0]);

    free(a.data);
    return 0;
}
*/
