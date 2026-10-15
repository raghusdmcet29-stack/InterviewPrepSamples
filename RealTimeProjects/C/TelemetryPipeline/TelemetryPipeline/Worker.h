//
//  Worker.h
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//
#ifndef WORKER_H
#define WORKER_H

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "Reading.h"
#include "BoundedQueue.h"
#include "StatsStore.h"

static pthread_mutex_t print_m = PTHREAD_MUTEX_INITIALIZER;

typedef struct {
    int id;
    BoundedQueue *queue;
    StatsStore *stats;
} WorkerArgs;

static inline void *worker_run(void *arg) {
    WorkerArgs *w = (WorkerArgs *)arg;
    Reading r;
    while (bq_dequeue(w->queue, &r)) {
        pthread_mutex_lock(&print_m);
        printf("worker %d processed %s: %.1f\n", w->id, r.deviceID, r.value);
        pthread_mutex_unlock(&print_m);

        ss_record(w->stats, &r);
        usleep(50000);
    }
    pthread_mutex_lock(&print_m);
    printf("worker %d stopped\n", w->id);
    pthread_mutex_unlock(&print_m);
    return NULL;
}

#endif
