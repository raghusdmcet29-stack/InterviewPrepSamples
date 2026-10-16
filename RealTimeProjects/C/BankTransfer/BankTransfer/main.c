//
//  main.c
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "Bank.h"
#include "Queue.h"
#include "Barrier.h"
#include "TokenBucket.h"
#include "Retry.h"
#include "AuditID.h"
#include "Summary.h"

typedef struct {
    Bank* bank;
    int from;
    int to;
    long long amount;
    int count;
} TransferJob;

static void* run_transfers(void* arg) {
    TransferJob* job = (TransferJob*)arg;
    for (int i = 0; i < job->count; i++) {
        TransferRequest r = {job->from, job->to, job->amount};
        bank_transfer(job->bank, r);
    }
    return NULL;
}

// The data one transfer attempt needs, passed to with_retry as ctx
typedef struct {
    Bank* bank;
    TransferRequest request;
} TransferAttempt;

static int attempt_transfer(void* ctx) {
    TransferAttempt* t = (TransferAttempt*)ctx;
    return bank_transfer(t->bank, t->request);
}

typedef struct {
    Bank* bank;
    BoundedQueue* queue;
    Barrier* start;
    TokenBucket* bucket;
    int id;
} WorkerJob;

static void* worker(void* arg) {
    WorkerJob* job = (WorkerJob*)arg;
    snprintf(auditID, sizeof auditID, "worker-%d", job->id);   // this thread's own ID

    printf("Worker %d arrived\n", job->id);
    barrier_wait(job->start);
    printf("Worker %d released\n", job->id);
    while (1) {
        QueueItem item = queue_dequeue(job->queue);
        if (item.is_stop) break;
        bucket_take(job->bucket);
        TransferAttempt attempt = {job->bank, item.request};
        with_retry(3, 100, attempt_transfer, &attempt);
        printf("[%s] moved %lld from %d to %d\n",
               auditID, item.request.amount, item.request.from, item.request.to);
    }
    printf("Worker %d stopped\n", job->id);
    return NULL;
}
// Stage 8 test: fails twice, then succeeds
static int fail_twice(void* ctx) {
    int* tries = (int*)ctx;
    (*tries)++;
    printf("Attempt %d\n", *tries);
    return *tries == 3;
}

int main(void) {
    Bank bank;
    bank_init(&bank);
    bank_open(&bank, 1, 1000);
    bank_open(&bank, 2, 1000);

    // Stage 1: two threads transfer in opposite directions
    TransferJob j1 = {&bank, 1, 2, 1, 1000};
    TransferJob j2 = {&bank, 2, 1, 1, 1000};
    pthread_t t1, t2;
    pthread_create(&t1, NULL, run_transfers, &j1);
    pthread_create(&t2, NULL, run_transfers, &j2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    long long a = bank_balance_of(&bank, 1);
    long long b = bank_balance_of(&bank, 2);
    printf("== Stage 1-4 results ==\n");
    printf("A: %lld, B: %lld, total: %lld\n", a, b, a + b);
    printf("Successful transfers: %lld\n", (long long)atomic_load(&bank.successCount));
    printf("Failed transfers: %lld\n", (long long)atomic_load(&bank.failedCount));

    // Stage 5-7: worker pool with rate limit
    printf("== Stage 5-7 ==\n");
    BoundedQueue queue;
    queue_init(&queue, 3);
    Barrier start;
    barrier_init(&start, 2);
    TokenBucket bucket;
    bucket_init(&bucket, 2, 10);

    struct timespec t_start, t_end;
    clock_gettime(CLOCK_MONOTONIC, &t_start);

    WorkerJob w1 = {&bank, &queue, &start, &bucket, 1};
    WorkerJob w2 = {&bank, &queue, &start, &bucket, 2};
    pthread_t wt1, wt2;
    pthread_create(&wt1, NULL, worker, &w1);
    pthread_create(&wt2, NULL, worker, &w2);

    for (int i = 0; i < 10; i++) {
        QueueItem item = {{1, 2, 10}, 0};
        queue_enqueue(&queue, item);
    }
    QueueItem stop = {{0, 0, 0}, 1};
    queue_enqueue(&queue, stop);
    queue_enqueue(&queue, stop);

    pthread_join(wt1, NULL);
    pthread_join(wt2, NULL);

    clock_gettime(CLOCK_MONOTONIC, &t_end);
    double seconds = (t_end.tv_sec - t_start.tv_sec)
                   + (t_end.tv_nsec - t_start.tv_nsec) / 1e9;
    printf("Batch took %.1f seconds\n", seconds);

    a = bank_balance_of(&bank, 1);
    b = bank_balance_of(&bank, 2);
    printf("A: %lld, B: %lld, total: %lld\n", a, b, a + b);
    printf("Successful transfers: %lld\n", (long long)atomic_load(&bank.successCount));
    printf("Failed transfers: %lld\n", (long long)atomic_load(&bank.failedCount));

    // Stage 8 test
    printf("== Stage 8 test ==\n");
    int tries = 0;
    int ok = with_retry(3, 100, fail_twice, &tries);
    printf("Succeeded: %s after %d attempts\n", ok ? "true" : "false", tries);
    
    // Stage 10: summary with one thread per account
    printf("== Stage 10 summary ==\n");
    int ids[2] = {1, 2};
    BalanceTask results[2];
    long long total = summarize(&bank, ids, results, 2);
    for (int i = 0; i < 2; i++) {
        printf("Account %d: %lld\n", results[i].id, results[i].balance);
    }
    printf("Total: %lld\n", total);
    printf("Failed transfers: %lld\n", (long long)atomic_load(&bank.failedCount));
    return 0;
}
