//
//  TokenBucket.h
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#ifndef TokenBucket_h
#define TokenBucket_h

#include <stdio.h>
#pragma once
#include <pthread.h>
#include <time.h>

typedef struct {
    double capacity;
    double refill_per_second;
    double tokens;
    struct timespec last;
    pthread_mutex_t mu;
} TokenBucket;

void bucket_init(TokenBucket* b, double capacity, double refill_per_second);
void bucket_take(TokenBucket* b);

#endif /* TokenBucket_h */
