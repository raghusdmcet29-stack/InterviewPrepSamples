//
//  main.c
//  VariadicPrintf
//
//  Created by Anussha on 31/08/26.
//

#include <stdio.h>
#include <stdarg.h>

// myPrintf supports only %d, %s, %c, %% — everything else is left as-is
void myPrintf(const char* format, ...) {
    va_list args;
    va_start(args, format);   // must start right after the last named parameter

    for (const char* p = format; *p != '\0'; p++) {
        if (*p != '%') {
            putchar(*p);
            continue;
        }

        p++; // move past '%'
        switch (*p) {
            case 'd': {
                int i = va_arg(args, int);      // pull next arg AS an int
                printf("%d", i);
                break;
            }
            case 's': {
                const char* s = va_arg(args, const char*);  // pull next arg AS a char*
                printf("%s", s);
                break;
            }
            case 'c': {
                // NOTE: char is promoted to int in variadic calls (default argument promotion)
                int c = va_arg(args, int);
                putchar(c);
                break;
            }
            case '%':
                putchar('%');
                break;
            default:
                putchar('%');
                putchar(*p);
        }
    }

    va_end(args);  // must call before returning
}

int main(void) {
    myPrintf("Name: %s, Age: %d, Grade: %c%%\n", "Anuraag", 45, 'A');
    // Now deliberately mismatch types against the format string
       // myPrintf("Bad call: %s\n", 12345);        // told it's a string, actually passed an int
      //  myPrintf("Bad call: %d\n", "hello");      // told it's an int, actually passed a pointer

    return 0;
}
