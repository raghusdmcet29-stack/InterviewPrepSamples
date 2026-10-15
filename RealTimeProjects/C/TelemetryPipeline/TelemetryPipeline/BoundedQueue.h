//
//  BoundedQueue.h
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#ifndef BOUNDED_QUEUE_H
#define BOUNDED_QUEUE_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Reading.h"

typedef struct {
    Reading *items;
    size_t capacity;
    size_t head;
    size_t count;
    bool closed;
    pthread_mutex_t m;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} BoundedQueue;

static inline void bq_init(BoundedQueue *q, size_t capacity) {
    q->items = malloc(sizeof(Reading) * capacity);
    q->capacity = capacity;
    q->head = 0;
    q->count = 0;
    q->closed = false;
    pthread_mutex_init(&q->m, NULL);
    pthread_cond_init(&q->not_full, NULL);
    pthread_cond_init(&q->not_empty, NULL);
}

static inline void bq_enqueue(BoundedQueue *q, Reading r) {
    pthread_mutex_lock(&q->m);
    while (q->count == q->capacity && !q->closed)
        pthread_cond_wait(&q->not_full, &q->m);
    if (!q->closed) {
        q->items[(q->head + q->count) % q->capacity] = r;
        q->count++;
        pthread_cond_signal(&q->not_empty);
    }
    pthread_mutex_unlock(&q->m);
}

static inline bool bq_dequeue(BoundedQueue *q, Reading *out) {
    pthread_mutex_lock(&q->m);
    while (q->count == 0 && !q->closed)
        pthread_cond_wait(&q->not_empty, &q->m);
    if (q->count == 0) {
        pthread_mutex_unlock(&q->m);
        return false;
    }
    *out = q->items[q->head];
    q->head = (q->head + 1) % q->capacity;
    q->count--;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->m);
    return true;
}

static inline void bq_close(BoundedQueue *q) {
    pthread_mutex_lock(&q->m);
    q->closed = true;
    pthread_cond_broadcast(&q->not_empty);
    pthread_cond_broadcast(&q->not_full);
    pthread_mutex_unlock(&q->m);
}

#endif
