//
//  main.c
//  AsyncAwait
//
//  Created by Anussha on 08/10/26.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *fetchNumber(void *arg) {
    int *result = (int *)arg;
    sleep(1);
    *result = 7;
    return NULL;
}

int main(void) {
    printf("Start\n");
    int a = 0;
    int b = 0;
    pthread_t t1;
    pthread_t t2;
    pthread_create(&t1, NULL, fetchNumber, &a);
    pthread_create(&t2, NULL, fetchNumber, &b);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Got %d\n", a + b);
    return 0;
}



/*
int main(void) {
    printf("Start\n");
    int result = 0;
    pthread_t t;
    pthread_create(&t, NULL, fetchNumber, &result);
    printf("Main continues\n");
    pthread_join(t, NULL);
    printf("Result %d\n", result);
    return 0;
}
*/
