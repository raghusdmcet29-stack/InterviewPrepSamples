//
//  main.c
//  AtomicCounter
//
//  Created by Anussha on 07/10/26.
//
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <time.h>

#define THREAD_COUNT 4
#define INCREMENTS_PER_THREAD 1000000

int mutexValue = 0;
pthread_mutex_t mutexLock = PTHREAD_MUTEX_INITIALIZER;

atomic_int atomicValue = 0;

void *mutexWorker(void *arg) {
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        pthread_mutex_lock(&mutexLock);
        mutexValue += 1;
        pthread_mutex_unlock(&mutexLock);
    }
    return NULL;
}

void *atomicWorker(void *arg) {
    for (int i = 0; i < INCREMENTS_PER_THREAD; i++) {
        atomic_fetch_add(&atomicValue, 1);
    }
    return NULL;
}

double runTest(void *(*worker)(void *)) {
    struct timespec start, end;
    pthread_t threads[THREAD_COUNT];

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int t = 0; t < THREAD_COUNT; t++) {
        pthread_create(&threads[t], NULL, worker, NULL);
    }
    for (int t = 0; t < THREAD_COUNT; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main(void) {
    double mutexTime = runTest(mutexWorker);
    printf("Mutex counter: %d (expected %d)\n", mutexValue, THREAD_COUNT * INCREMENTS_PER_THREAD);
    printf("Mutex time: %f seconds\n", mutexTime);

    double atomicTime = runTest(atomicWorker);
    printf("Atomic counter: %d (expected %d)\n", atomic_load(&atomicValue), THREAD_COUNT * INCREMENTS_PER_THREAD);
    printf("Atomic time: %f seconds\n", atomicTime);

    return 0;
}
