//
//  main.c
//  UndefinedBehavior1-SignedOverflow
//
//  Created by Anussha on 09/10/26.
//

#include <stdio.h>
#include <limits.h>

int main(void) {
    int big = INT_MAX;
    if (big + 1 < big) {
        printf("overflow detected\n");
    } else {
        printf("no overflow detected\n");
    }
    return 0;
}
