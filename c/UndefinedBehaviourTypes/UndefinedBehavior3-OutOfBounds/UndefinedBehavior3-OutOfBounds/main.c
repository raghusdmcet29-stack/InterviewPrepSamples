//
//  main.c
//  UndefinedBehavior3-OutOfBounds
//
//  Created by Anussha on 09/10/26.
//

#include <stdio.h>

int main(void) {
    int numbers[3] = {10, 20, 30};
    printf("%d\n", numbers[3]);
    return 0;
}
