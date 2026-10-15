//
//  main.c
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//
#include <pthread.h>
#include <stdio.h>
#include "Reading.h"
#include "BoundedQueue.h"
#include "Worker.h"

static BoundedQueue queue;

static void send(const char *device, double value) {
    Reading r;
    snprintf(r.deviceID, sizeof r.deviceID, "%s", device);
    r.value = value;
    bq_enqueue(&queue, r);
}

int main(void) {
    bq_init(&queue, 4);

    pthread_t workers[2];
    WorkerArgs args[2];
    for (int i = 0; i < 2; i++) {
        args[i].id = i + 1;
        args[i].queue = &queue;
        pthread_create(&workers[i], NULL, worker_run, &args[i]);
    }

    double values[] = {21.0, 23.0, 22.0};
    for (int i = 0; i < 3; i++) {
        send("sensor-1", values[i]);
        send("sensor-2", values[i] + 10);
    }

    bq_close(&queue);
    for (int i = 0; i < 2; i++) {
        pthread_join(workers[i], NULL);
    }
    return 0;
}
