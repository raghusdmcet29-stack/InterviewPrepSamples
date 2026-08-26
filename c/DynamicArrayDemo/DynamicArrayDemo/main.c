//
//  main.c
//  DynamicArrayDemo
//
//  Created by Anussha on 25/08/26.
//

#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int* data;
    int size; // how many elements are actually in use
    int capacity; // how many elements the buffer can currently hold
}IntArray;

IntArray createArray(int intialCapacity){
    IntArray arr;
    arr.data =(int*)malloc(intialCapacity * sizeof(int));
    arr.size = 0;
    arr.capacity = intialCapacity;
    printf("Array created with capacity %d\n", arr.capacity);
    return arr;
}

void pushBack(IntArray* arr,int value){
    if (arr->size == arr->capacity) {
        int newCapacity = arr->capacity * 2;
        arr->data = (int*)realloc(arr->data, newCapacity * sizeof(int));
        arr->capacity = newCapacity;
        printf("Grew capacity to %d\n", newCapacity);
    }
    arr->data[arr->size] = value;
    arr->size++;
}

void freeArray(IntArray* arr) {
    free(arr->data);
    arr->data = NULL;   // avoid a dangling pointer
    arr->size = 0;
    arr->capacity = 0;
    printf("Array freed\n");
}

int main(void) {
    IntArray arr = createArray(2);

    for (int i = 1; i <= 5; i++) {
        pushBack(&arr, i * 10);
    }

    printf("Final size: %d, capacity: %d\n", arr.size, arr.capacity);
    for (int i = 0; i < arr.size; i++) {
        printf("arr[%d] = %d\n", i, arr.data[i]);
    }

    freeArray(&arr);
   
    return 0;
}
