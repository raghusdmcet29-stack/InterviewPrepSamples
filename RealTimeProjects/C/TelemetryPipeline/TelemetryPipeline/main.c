//
//  main.c
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "Reading.h"
#include "BoundedQueue.h"
#include "StatsStore.h"
#include "Worker.h"

static BoundedQueue queue;
static StatsStore stats;

static void send(const char *device, double value) {
    Reading r;
    snprintf(r.deviceID, sizeof r.deviceID, "%s", device);
    r.value = value;
    bq_enqueue(&queue, r);
}

static void *dashboard_run(void *arg) {
    (void)arg;
    const char *devices[] = {"sensor-1", "sensor-2"};
    for (int tick = 1; tick <= 10; tick++) {
        usleep(20000);
        for (int d = 0; d < 2; d++) {
            DeviceStats s;
            if (ss_snapshot(&stats, devices[d], &s)) {
                pthread_mutex_lock(&print_m);
                printf("dashboard tick %d: %s count=%d\n", tick, devices[d], s.count);
                pthread_mutex_unlock(&print_m);
            }
        }
    }
    return NULL;
}

int main(void) {
    bq_init(&queue, 4);
    ss_init(&stats);

    pthread_t workers[2];
    WorkerArgs args[2];
    for (int i = 0; i < 2; i++) {
        args[i].id = i + 1;
        args[i].queue = &queue;
        args[i].stats = &stats;
        pthread_create(&workers[i], NULL, worker_run, &args[i]);
    }

    pthread_t dashboard;
    pthread_create(&dashboard, NULL, dashboard_run, NULL);

    double values[] = {21.0, 23.0, 22.0};
    for (int i = 0; i < 3; i++) {
        send("sensor-1", values[i]);
        send("sensor-2", values[i] + 10);
    }

    bq_close(&queue);
    for (int i = 0; i < 2; i++) {
        pthread_join(workers[i], NULL);
    }
    pthread_join(dashboard, NULL);

    const char *devices[] = {"sensor-1", "sensor-2"};
    for (int d = 0; d < 2; d++) {
        DeviceStats s;
        if (ss_snapshot(&stats, devices[d], &s)) {
            printf("%s count=%d min=%.1f max=%.1f avg=%.1f\n",
                   devices[d], s.count, s.min, s.max, ds_average(&s));
        }
    }
    return 0;
}
