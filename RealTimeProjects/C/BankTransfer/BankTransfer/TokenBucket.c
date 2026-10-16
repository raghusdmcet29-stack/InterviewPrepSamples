//
//  TokenBucket.c
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#include <time.h>
#include "TokenBucket.h"

static double seconds_between(struct timespec from, struct timespec to) {
    return (to.tv_sec - from.tv_sec) + (to.tv_nsec - from.tv_nsec) / 1e9;
}

// Caller must hold b->mu
static void refill(TokenBucket* b) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    double elapsed = seconds_between(b->last, now);
    b->tokens += elapsed * b->refill_per_second;
    if (b->tokens > b->capacity) b->tokens = b->capacity;
    b->last = now;
}

void bucket_init(TokenBucket* b, double capacity, double refill_per_second) {
    b->capacity = capacity;
    b->refill_per_second = refill_per_second;
    b->tokens = capacity;
    clock_gettime(CLOCK_MONOTONIC, &b->last);
    pthread_mutex_init(&b->mu, NULL);
}

// Blocks until one token is available, then takes it
void bucket_take(TokenBucket* b) {
    while (1) {
        pthread_mutex_lock(&b->mu);
        refill(b);
        if (b->tokens >= 1.0) {
            b->tokens -= 1.0;
            pthread_mutex_unlock(&b->mu);
            return;
        }
        pthread_mutex_unlock(&b->mu);

        struct timespec pause = {0, 10000000};   // sleep 10 ms, then check again
        nanosleep(&pause, NULL);
    }
}
