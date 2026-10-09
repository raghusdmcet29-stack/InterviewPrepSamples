//
//  main.c
//  offsetof
//
//  Created by Anussha on 09/10/26.
//

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

struct Inner {
    int8_t a;
    int32_t b;
};

struct Outer {
    int8_t x;
    struct Inner inner;
};

int main(void) {
    printf("x at offset %zu\n", offsetof(struct Outer, x));
    printf("inner at offset %zu\n", offsetof(struct Outer, inner));
    printf("inner.b at offset %zu\n", offsetof(struct Outer, inner.b));
    return 0;
}
