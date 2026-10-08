//
//  main.c
//  MemoryLayout1-BasicTypes
//
//  Created by Anussha on 08/10/26.
//

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>

int main(void) {
    printf("int8_t  size: %zu alignment: %zu\n", sizeof(int8_t),  alignof(int8_t));
    printf("int32_t size: %zu alignment: %zu\n", sizeof(int32_t), alignof(int32_t));
    printf("int64_t size: %zu alignment: %zu\n", sizeof(int64_t), alignof(int64_t));
    printf("bool    size: %zu alignment: %zu\n", sizeof(bool),    alignof(bool));
    printf("double  size: %zu alignment: %zu\n", sizeof(double),  alignof(double));
    return 0;
}
