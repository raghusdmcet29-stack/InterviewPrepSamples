//
//  main.c
//  SingletonPattern
//
//  Created by Anussha on 11/09/26.
//
#include <stdio.h>

typedef struct {
    char environment[32];
    int maxRetries;
} AppConfig;

static AppConfig instance = { "production", 3 };

AppConfig* AppConfig_shared(void) {
    return &instance;
}

int main(void) {
    AppConfig* a = AppConfig_shared();
    AppConfig* b = AppConfig_shared();

    printf("%s\n", a->environment);
    printf("%d\n", a->maxRetries);

    a->maxRetries = 10;
    printf("%d\n", b->maxRetries);
    printf("%d\n", a == b);
    AppConfig another = { "staging", 5 }; // perfectly legal, compiles fine
    return 0;
}
