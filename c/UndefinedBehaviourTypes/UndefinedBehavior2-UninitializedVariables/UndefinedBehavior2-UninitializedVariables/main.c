//
//  main.c
//  UndefinedBehavior2-UninitializedVariables
//
//  Created by Anussha on 09/10/26.
//
#include <stdio.h>

int main(void) {
    int count = 0;
    printf("%d\n", count);
    return 0;
}
/*
#include <stdio.h>

int main(void) {
    int count;
    printf("%d\n", count);
    return 0;
}
*/
