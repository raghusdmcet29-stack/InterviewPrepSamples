//
//  main.c
//  Deadlock
//
//  Created by Anussha on 05/10/26.
//
// dead lock avoid using locks in order both the threads

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void *threadA(void *arg) {
    pthread_mutex_lock(&lock1);
    printf("A took Lock 1\n");
    sleep(1);
    printf("A wants Lock 2\n");
    pthread_mutex_lock(&lock2);
    printf("A took Lock 2\n");
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void *threadB(void *arg) {
    pthread_mutex_lock(&lock1);                  // CHANGED: was lock2
    printf("B took Lock 1\n");                   // CHANGED
    sleep(1);
    printf("B wants Lock 2\n");                  // CHANGED
    pthread_mutex_lock(&lock2);                  // CHANGED: was lock1
    printf("B took Lock 2\n");                   // CHANGED
    pthread_mutex_unlock(&lock2);                // CHANGED
    pthread_mutex_unlock(&lock1);                // CHANGED
    return NULL;
}

int main(void) {
    pthread_t a, b;
    pthread_create(&a, NULL, threadA, NULL);
    pthread_create(&b, NULL, threadB, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("Done\n");
    return 0;
}

// dead lock in c for 2 threads
/*#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER;

void *threadA(void *arg) {
    pthread_mutex_lock(&lock1);
    printf("A took Lock 1\n");
    sleep(1);
    printf("A wants Lock 2\n");
    pthread_mutex_lock(&lock2);
    printf("A took Lock 2\n");
    pthread_mutex_unlock(&lock2);
    pthread_mutex_unlock(&lock1);
    return NULL;
}

void *threadB(void *arg) {
    pthread_mutex_lock(&lock2);
    printf("B took Lock 2\n");
    sleep(1);
    printf("B wants Lock 1\n");
    pthread_mutex_lock(&lock1);
    printf("B took Lock 1\n");
    pthread_mutex_unlock(&lock1);
    pthread_mutex_unlock(&lock2);
    return NULL;
}

int main(void) {
    pthread_t a, b;
    pthread_create(&a, NULL, threadA, NULL);
    pthread_create(&b, NULL, threadB, NULL);
    pthread_join(a, NULL);
    pthread_join(b, NULL);
    printf("Done\n");
    return 0;
}
*/
