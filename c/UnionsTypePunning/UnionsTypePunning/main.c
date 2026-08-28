//
//  main.c
//  UnionsTypePunning
//
//  Created by Anussha on 28/08/26.
//

#include <stdio.h>

union Data {
    int i;
    float f;
    char bytes[4];
};

int isLittleEndian(void) {
    union Data d;
    d.i = 1;
    return d.bytes[0] == 1; // if the LSB (1) is in byte[0], it's little-endian
}

struct DataStruct {
    int i;
    float f;
    char bytes[4];
};



int main(void){
    union Data d;
    
    printf("sizeof(union Data) = %zu bytes\n", sizeof(union Data));
    
    d.i = 65;
    printf("As int: %d\n", d.i);
    printf("As bytes: %d %d %d %d\n", d.bytes[0], d.bytes[1], d.bytes[2], d.bytes[3]);

    if (isLittleEndian()) {
            printf("This machine is LITTLE-endian\n");
        } else {
            printf("This machine is BIG-endian\n");
        }
    printf("sizeof(struct DataStruct) = %zu bytes\n", sizeof(struct DataStruct));
    return 0;
}
