//
//  main.c
//  BitmasksAndBitfields
//
//  Created by Anussha on 28/08/26.
//

#include <stdio.h>

/*#define FLAG_READ    (1 << 0)  // 0001
#define FLAG_WRITE   (1 << 1)  // 0010
#define FLAG_EXECUTE (1 << 2)  // 0100
#define FLAG_DELETE  (1 << 3)  // 1000


int main(void) {
    unsigned int permissions = 0; // start with no flags set

    // Set READ and WRITE
    permissions |= FLAG_READ;
    permissions |= FLAG_WRITE;

    printf("Permissions after setting READ, WRITE: %d\n", permissions);

    // Check if READ is set
    if (permissions & FLAG_READ) {
        printf("READ is set\n");
    }

    // Check if EXECUTE is set (should not be)
    if (permissions & FLAG_EXECUTE) {
        printf("EXECUTE is set\n");
    } else {
        printf("EXECUTE is NOT set\n");
    }
    
    // Clear WRITE
    permissions &= ~FLAG_WRITE;
    printf("Permissions after clearing WRITE: %d\n", permissions);

    // Toggle EXECUTE (was off, should turn on)
    permissions ^= FLAG_EXECUTE;
    printf("Permissions after toggling EXECUTE: %d\n", permissions);

    // Toggle EXECUTE again (was on, should turn off)
    permissions ^= FLAG_EXECUTE;
    printf("Permissions after toggling EXECUTE again: %d\n", permissions);

    return 0;
}
*/
// structure based approach

#include <stdio.h>

// Naive: each flag is a full int/bool
struct PermissionsNaive {
    int read;
    int write;
    int execute;
    int delete_;
};

// Bitfield: compiler packs all 4 flags into as few bits as possible
struct PermissionsBitfield {
    unsigned int read    : 1;
    unsigned int write   : 1;
    unsigned int execute : 1;
    unsigned int delete_ : 1;
};

struct PermissionsBitfieldChar {
    unsigned char read    : 1;
    unsigned char write   : 1;
    unsigned char execute : 1;
    unsigned char delete_ : 1;
};

int main(void) {
    printf("sizeof(PermissionsNaive) = %zu bytes\n", sizeof(struct PermissionsNaive));
    printf("sizeof(PermissionsBitfield) = %zu bytes\n", sizeof(struct PermissionsBitfield));
    printf("sizeof(PermissionsBitfieldChar) = %zu bytes\n", sizeof(struct PermissionsBitfieldChar));
    struct PermissionsBitfield p = {0};
    p.read = 1;
    p.write = 1;

    printf("read=%u write=%u execute=%u delete=%u\n", p.read, p.write, p.execute, p.delete_);

    return 0;
}
