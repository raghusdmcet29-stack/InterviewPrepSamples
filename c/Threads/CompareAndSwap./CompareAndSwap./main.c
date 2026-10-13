//
//  main.c
//  CompareAndSwap.
//
//  Created by Anussha on 13/10/26.
//

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

atomic_int value = 0;

void add_one(void) {
    int expected = atomic_load(&value);
    while (!atomic_compare_exchange_strong(&value, &expected, expected + 1)) {
        /* on failure, expected now holds the latest value, so the loop retries with it */
    }
}

void *worker(void *arg) {
    (void)arg;
    for (int i = 0; i < 1000; i++) {
        add_one();
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, NULL);
    pthread_create(&t2, NULL, worker, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Final value: %d\n", atomic_load(&value));
    return 0;
}


/*
#include <stdatomic.h>
#include <stdio.h>

int main(void) {
    atomic_int value = 5;

    int expected = 5;
    int ok1 = atomic_compare_exchange_strong(&value, &expected, 6);
    printf("First CAS: %d, value now %d\n", ok1, atomic_load(&value));

    expected = 5;
    int ok2 = atomic_compare_exchange_strong(&value, &expected, 7);
    printf("Second CAS: %d, value now %d, expected now %d\n", ok2, atomic_load(&value), expected);

    return 0;
}
*/
