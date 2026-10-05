//
//  main.c
//  ProducerConsumer
//
//  Created by Anussha on 05/10/26.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
int queue[10];
int head = 0;
int tail = 0;

void *consumer(void *arg) {
    for (int i = 0; i < 3; i++) {
        pthread_mutex_lock(&m);
        while (head == tail) {
            pthread_cond_wait(&cv, &m);
        }
        int item = queue[head];
        head++;
        printf("Consumer took %d\n", item);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

void *producer(void *arg) {
    for (int item = 1; item <= 3; item++) {
        sleep(1);
        pthread_mutex_lock(&m);
        queue[tail] = item;
        tail++;
        printf("Producer added %d\n", item);
        pthread_cond_signal(&cv);
        pthread_mutex_unlock(&m);
    }
    return NULL;
}

int main(void) {
    pthread_t c, p;
    pthread_create(&c, NULL, consumer, NULL);
    pthread_create(&p, NULL, producer, NULL);
    pthread_join(c, NULL);
    pthread_join(p, NULL);
    printf("Done\n");
    return 0;
}

/*
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
bool ready = false;

void *waiter(void *arg) {
    pthread_mutex_lock(&m);
    printf("Waiter: waiting\n");
    while (!ready) {
        pthread_cond_wait(&cv, &m);
    }
    printf("Waiter: woke up\n");
    pthread_mutex_unlock(&m);
    return NULL;
}

int main(void) {
    pthread_t t;
    pthread_create(&t, NULL, waiter, NULL);
    sleep(1);
    pthread_mutex_lock(&m);
    ready = true;
    printf("Main: signalling\n");
    pthread_cond_signal(&cv);
    pthread_mutex_unlock(&m);
    pthread_join(t, NULL);
    printf("Done\n");
    return 0;
}
*/
