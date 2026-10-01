//
//  main.c
//  DanglingPointers
//
//  Created by Anussha on 01/10/26.
//

// fix to dangling pointer

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = malloc(sizeof(int));
    *p = 42;
    int *q = p;

    printf("Before free: %d\n", *p);

    free(p);
    p = NULL;  // no dangling pointer

    printf("Freed.\n");

    if (p != NULL) {
        printf("p after free: %d\n", *p);
    } else {
        printf("p is NULL, read skipped\n");
    }

    if (q != NULL) { // dangling pointer still
        printf("q after free: %d\n", *q);
    } else {
        printf("q is NULL, read skipped\n");
    }

    return 0;
}

// dangling pointer example
/*#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int *p = malloc(sizeof(int));
    *p = 42;

    printf("Before free: %d\n", *p);

    free(p);
   

    printf("Freed.\n");
    printf("After free: %d\n", *p);

    return 0;
}
*/
