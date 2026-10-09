//
//  main.c
//  UndefinedBehavior4-StrictAliasing
//
//  Created by Anussha on 09/10/26.
//

#include <stdio.h>
#include <string.h>

int main(void) {
    float f = 1.5f;
    int bits;
    memcpy(&bits, &f, sizeof(bits));
    printf("%d\n", bits);
    return 0;
}
