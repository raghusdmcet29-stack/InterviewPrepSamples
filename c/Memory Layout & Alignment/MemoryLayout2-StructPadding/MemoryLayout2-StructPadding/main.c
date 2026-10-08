//
//  main.c
//  MemoryLayout2-StructPadding
//
//  Created by Anussha on 08/10/26.
//
#include <stdio.h>
#include <stdint.h>

struct Mixed {
    int8_t a;
    int32_t b;
    int8_t c;
};

struct Reordered {
    int32_t b;
    int8_t a;
    int8_t c;
};

int main(void) {
    printf("Mixed     sizeof: %zu alignof: %zu\n", sizeof(struct Mixed), _Alignof(struct Mixed));
    printf("Reordered sizeof: %zu alignof: %zu\n", sizeof(struct Reordered), _Alignof(struct Reordered));
    return 0;
}
