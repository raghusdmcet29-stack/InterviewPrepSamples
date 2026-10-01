//
//  main.c
//  StackVsHeap
//
//  Created by Anussha on 01/10/26.
//

#include <stdio.h>
#include <stdlib.h>

int *makeOnHeap(void) {
    int *box = malloc(sizeof(int));
    *box = 10;
    printf("inside makeOnHeap:  %p\n", (void *)box);
    printf("makeOnHeap ending\n");
    return box;
}

void caller(void) {
    int *b = makeOnHeap();
    printf("outside makeOnHeap: %p\n", (void *)b);
    printf("value: %d\n", *b);
    printf("caller ending\n");
    free(b);
}

int main(void) {
    caller();
    printf("after caller\n");
    return 0;
}
/*
#include <stdio.h>
#include <stdlib.h>

int *makeOnStack(void) {
    int x = 10;
    return &x;
}

int main(void) {
    int *p = makeOnStack();
    printf("p points to: %p\n", (void *)p);
    printf("value: %d\n", *p);
    return 0;
}*/
/*
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int x = 10;
    int *h = malloc(sizeof(int));
    *h = 20;

    printf("x address (stack): %p\n", (void *)&x);
    printf("h points to (heap): %p\n", (void *)h);
    printf("h itself (stack): %p\n", (void *)&h);

    free(h);
    return 0;
}
*/
