//
//  Worker.h
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//
#ifndef WORKER_H
#define WORKER_H

#include <stdio.h>
#include "Reading.h"
#include "BoundedQueue.h"

typedef struct {
    int id;
    BoundedQueue *queue;
} WorkerArgs;

static inline void *worker_run(void *arg) {
    WorkerArgs *w = (WorkerArgs *)arg;
    Reading r;
    while (bq_dequeue(w->queue, &r)) {
        printf("worker %d processed %s: %.1f\n", w->id, r.deviceID, r.value);
    }
    printf("worker %d stopped\n", w->id);
    return NULL;
}

#endif
