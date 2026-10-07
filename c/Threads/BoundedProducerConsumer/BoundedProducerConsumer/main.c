//
//  main.c
//  BoundedProducerConsumer
//
//  Created by Anussha on 07/10/26.
//

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define CAPACITY 2

typedef struct {
    int items[16];
    int head;
    int tail;
    pthread_mutex_t m;
    pthread_cond_t cv;
} BoundedQueue;

BoundedQueue queue = { .head = 0, .tail = 0,
                       .m = PTHREAD_MUTEX_INITIALIZER,
                       .cv = PTHREAD_COND_INITIALIZER };

void queueAdd(int item) {
    pthread_mutex_lock(&queue.m);
    while (queue.tail - queue.head == CAPACITY) {
        printf("Queue full, producer waiting\n");
        pthread_cond_wait(&queue.cv, &queue.m);
    }
    queue.items[queue.tail] = item;
    queue.tail++;
    printf("Added %d, queue size %d\n", item, queue.tail - queue.head);
    pthread_cond_signal(&queue.cv);
    pthread_mutex_unlock(&queue.m);
}

int queueTake(void) {
    pthread_mutex_lock(&queue.m);
    while (queue.head == queue.tail) {
        pthread_cond_wait(&queue.cv, &queue.m);
    }
    int item = queue.items[queue.head];
    queue.head++;
    printf("Took %d, queue size %d\n", item, queue.tail - queue.head);
    pthread_cond_signal(&queue.cv);
    pthread_mutex_unlock(&queue.m);
    return item;
}

void *consumerLoop(void *arg) {
    (void)arg;
    for (int i = 0; i < 5; i++) {
        sleep(1);
        queueTake();
    }
    return NULL;
}

int main(void) {
    pthread_t consumer;
    pthread_create(&consumer, NULL, consumerLoop, NULL);
    
    for (int i = 1; i <= 5; i++) {
        queueAdd(i);
    }
    printf("Producer finished\n");
    
    pthread_join(consumer, NULL);
    printf("Done\n");
    return 0;
}
