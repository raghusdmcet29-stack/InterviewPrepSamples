//
//  main.c
//  UndefinedBehavior5-InvalidPointers
//
//  Created by Anussha on 09/10/26.
//

#include <stdio.h>

int main(void) {
    int *p = NULL;
    printf("%d\n", *p);
    return 0;
}
