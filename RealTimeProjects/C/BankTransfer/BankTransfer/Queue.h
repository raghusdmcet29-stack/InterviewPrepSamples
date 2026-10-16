//
//  Queue.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef Queue_h
#define Queue_h

#include <stdio.h>
#pragma once
#include <pthread.h>
#include "Bank.h"

typedef struct {
    TransferRequest request;
    int is_stop;            // 1 = stop marker, like nil in Swift
} QueueItem;

typedef struct {
    QueueItem* items;
    int capacity;
    int head;
    int tail;
    int size;
    pthread_mutex_t mu;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
} BoundedQueue;

void queue_init(BoundedQueue* q, int capacity);
void queue_enqueue(BoundedQueue* q, QueueItem item);
QueueItem queue_dequeue(BoundedQueue* q);

#endif /* Queue_h */
