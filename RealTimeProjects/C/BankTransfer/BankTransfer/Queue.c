//
//  Queue.c
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#include <stdlib.h>
#include "Queue.h"

void queue_init(BoundedQueue* q, int capacity) {
    q->items = malloc(sizeof(QueueItem) * capacity);
    q->capacity = capacity;
    q->head = 0;
    q->tail = 0;
    q->size = 0;
    pthread_mutex_init(&q->mu, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
}

void queue_enqueue(BoundedQueue* q, QueueItem item) {
    pthread_mutex_lock(&q->mu);
    while (q->size == q->capacity) {
        pthread_cond_wait(&q->not_full, &q->mu);
    }
    q->items[q->tail] = item;
    q->tail = (q->tail + 1) % q->capacity;
    q->size++;
    pthread_cond_signal(&q->not_empty);
    pthread_mutex_unlock(&q->mu);
}

QueueItem queue_dequeue(BoundedQueue* q) {
    pthread_mutex_lock(&q->mu);
    while (q->size == 0) {
        pthread_cond_wait(&q->not_empty, &q->mu);
    }
    QueueItem item = q->items[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->size--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mu);
    return item;
}
