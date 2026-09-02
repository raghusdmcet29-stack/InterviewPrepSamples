//
//  main.c
//  TokenBucketRateLimiter
//
//  Created by Anussha on 02/09/26.
//

#include <stdio.h>
// with out token bucket
/*
void makeRequest(int id) {
    printf("Request %d allowed\n", id);
}

int main(void) {
    for (int i = 1; i <= 10; i++) {
        makeRequest(i);
    }
    return 0;
}
*/

#include <time.h>
#include <stdbool.h>

typedef struct {
    double maxTokens;
    double tokens;
    double refillRatePerSecond;
    struct timespec lastRefillTime;
} TokenBucket;

static double timeDiffSeconds(struct timespec start, struct timespec end) {
    double sec = (double)(end.tv_sec - start.tv_sec);
    double nsec = (double)(end.tv_nsec - start.tv_nsec) / 1e9;
    return sec + nsec;
}

void bucketInit(TokenBucket* bucket, double maxTokens, double refillRatePerSecond) {
    bucket->maxTokens = maxTokens;
    bucket->tokens = maxTokens;
    bucket->refillRatePerSecond = refillRatePerSecond;
    clock_gettime(CLOCK_MONOTONIC, &bucket->lastRefillTime);
}

static void bucketRefill(TokenBucket* bucket) {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

    double elapsed = timeDiffSeconds(bucket->lastRefillTime, now);
    double newTokens = elapsed * bucket->refillRatePerSecond;

    bucket->tokens += newTokens;
    if (bucket->tokens > bucket->maxTokens) {
        bucket->tokens = bucket->maxTokens;
    }
    bucket->lastRefillTime = now;
}

bool bucketTryConsume(TokenBucket* bucket) {
    bucketRefill(bucket);
    if (bucket->tokens >= 1.0) {
        bucket->tokens -= 1.0;
        return true;
    }
    return false;
}

#include <stdio.h>
#include <unistd.h>  // for sleep()

int main(void) {
    TokenBucket bucket;
    bucketInit(&bucket, 5.0, 5.0);  // max 5 tokens, refill 5 tokens/second

    printf("--- Burst of 10 rapid requests ---\n");
    for (int i = 1; i <= 10; i++) {
        if (bucketTryConsume(&bucket)) {
            printf("Request %d allowed\n", i);
        } else {
            printf("Request %d REJECTED\n", i);
        }
    }

    printf("--- Waiting 1 second to refill ---\n");
    sleep(1);

    printf("--- Trying again after refill ---\n");
    for (int i = 11; i <= 13; i++) {
        if (bucketTryConsume(&bucket)) {
            printf("Request %d allowed\n", i);
        } else {
            printf("Request %d REJECTED\n", i);
        }
    }

    return 0;
}
