//
//  main.c
//  FunctionPointerDemo
//
//  Created by Anussha on 25/08/26.
//

#include <stdio.h>
#include <stdlib.h>

void forEach(int* arr,int size,void (*callback)(int)){
    for (int i=0; i<size; i++) {
        callback(arr[i]);
    }
}
void printSquare(int x) {
    printf("%d squared is %d\n", x, x * x);
}

void printDouble(int x) {
    printf("%d doubled is %d\n", x, x * 2);
}

int compareInts(const void* a, const void* b) {
    int intA = *(const int*)a;
    int intB = *(const int*)b;
    return intA - intB;
}

int main(void) {
    int numbers[5] = {1, 2, 3, 4, 5};

    printf("--- Using printSquare ---\n");
    forEach(numbers, 5, printSquare);

    printf("--- Using printDouble ---\n");
    forEach(numbers, 5, printDouble);
    
    int unsorted[5] = {40, 10, 30, 50, 20};

    printf("--- Before sort ---\n");
    forEach(unsorted, 5, printSquare);

    qsort(unsorted, 5, sizeof(int), compareInts);

    printf("--- After sort ---\n");
    forEach(unsorted, 5, printSquare);

    return 0;
}
