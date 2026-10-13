//
//  main.c
//  AsyncStreamDelivery
//
//  Created by Anussha on 13/10/26.
//

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;
int values[16];
int head = 0;
int tail = 0;
int finished = 0;

void *producer(void *arg) {
    (void)arg;
    for (int i = 1; i <= 3; i++) {
        sleep(1);
        pthread_mutex_lock(&m);
        values[tail] = i;
        tail++;
        pthread_mutex_unlock(&m);
        pthread_cond_signal(&cv);
    }
    pthread_mutex_lock(&m);
    finished = 1;
    pthread_mutex_unlock(&m);
    pthread_cond_signal(&cv);
    return NULL;
}

void *consumer(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&m);
        while (head == tail && !finished) {
            pthread_cond_wait(&cv, &m);
        }
        if (head == tail && finished) {
            pthread_mutex_unlock(&m);
            break;
        }
        int value = values[head];
        head++;
        pthread_mutex_unlock(&m);
        printf("Got %d\n", value);
    }
    printf("Stream finished\n");
    return NULL;
}

int main(void) {
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}
