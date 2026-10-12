//
//  main.c
//  SpinLock
//
//  Created by Anussha on 12/10/26.
//

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>

atomic_flag flag = ATOMIC_FLAG_INIT;
int counter = 0;

static pthread_mutex_t printMutex = PTHREAD_MUTEX_INITIALIZER;

void printLine(const char *text) {
    pthread_mutex_lock(&printMutex);
    printf("%s\n", text);
    pthread_mutex_unlock(&printMutex);
}

void spinLock(void) {
    while (atomic_flag_test_and_set(&flag)) {
        // spin until the old value was false
    }
}

void spinUnlock(void) {
    atomic_flag_clear(&flag);
}

void *work(void *arg) {
    const char *name = (const char *)arg;
    spinLock();
    counter++;
    char buf[64];
    snprintf(buf, sizeof buf, "%s added, counter = %d", name, counter);
    printLine(buf);
    spinUnlock();
    return NULL;
}

int main(void) {
    pthread_t t1, t2, t3;
    pthread_create(&t1, NULL, work, "T1");
    pthread_create(&t2, NULL, work, "T2");
    pthread_create(&t3, NULL, work, "T3");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    printLine("Done, final counter = 3");
    return 0;
}
