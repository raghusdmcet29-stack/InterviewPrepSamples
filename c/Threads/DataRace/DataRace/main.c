//
//  main.c
//  DataRace
//
//  Created by Anussha on 05/10/26.
//

// Avoiding data race in c using pthred mutex

#include <stdio.h>
#include <pthread.h>

int counter = 0;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;   // NEW: the mutex, ready to use

void *work(void *arg) {
    for (int i = 0; i < 50000; i++) {
        pthread_mutex_lock(&m);                  // NEW: take the lock
        counter += 1;
        pthread_mutex_unlock(&m);                // NEW: release the lock
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, work, NULL);
    pthread_create(&t2, NULL, work, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Expected: 100000\n");
    printf("Actual: %d\n", counter);
    return 0;
}

// data race in c
/*#include <stdio.h>
#include <pthread.h>

int counter = 0;

void *work(void *arg) {
    for (int i = 0; i < 50000; i++) {
        counter += 1;
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, work, NULL);
    pthread_create(&t2, NULL, work, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Expected: 100000\n");
    printf("Actual: %d\n", counter);
    return 0;
}
*/
// Thread creation in c
/*#include <stdio.h>
#include <pthread.h>

void *sayHello(void *arg) {
    printf("Hello from thread\n");
    return NULL;
}

int main(void) {
    pthread_t t;
    pthread_create(&t, NULL, sayHello, NULL);
    pthread_join(t, NULL);
    printf("Done\n");
    return 0;
}
*/
