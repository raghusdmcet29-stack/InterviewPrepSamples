//
//  main.c
//  MemoryLayout3-NestedStructs
//
//  Created by Anussha on 09/10/26.
//

#include <stdio.h>
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
    printf("Inner: sizeof %zu, alignof %zu\n", sizeof(struct Inner), _Alignof(struct Inner));
    printf("Outer: sizeof %zu, alignof %zu\n", sizeof(struct Outer), _Alignof(struct Outer));
    return 0;
}
