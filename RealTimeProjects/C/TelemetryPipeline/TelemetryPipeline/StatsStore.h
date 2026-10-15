//
//  StatsStore.h
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#ifndef STATS_STORE_H
#define STATS_STORE_H

#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include "DeviceStats.h"
#include "Reading.h"

#define MAX_DEVICES 8

typedef struct {
    char name[32];
    DeviceStats stats;
} DeviceEntry;

typedef struct {
    DeviceEntry entries[MAX_DEVICES];
    int used;
    pthread_rwlock_t lock;
} StatsStore;

static inline void ss_init(StatsStore *s) {
    s->used = 0;
    pthread_rwlock_init(&s->lock, NULL);
}

static inline void ss_record(StatsStore *s, const Reading *r) {
    pthread_rwlock_wrlock(&s->lock);   // writer: exclusive
    int i;
    for (i = 0; i < s->used; i++) {
        if (strcmp(s->entries[i].name, r->deviceID) == 0) break;
    }
    if (i == s->used && s->used < MAX_DEVICES) {
        snprintf(s->entries[i].name, sizeof s->entries[i].name, "%s", r->deviceID);
        ds_init(&s->entries[i].stats);
        s->used++;
    }
    if (i < s->used) ds_add(&s->entries[i].stats, r->value);
    pthread_rwlock_unlock(&s->lock);
}

// Returns true and copies the stats if the device exists.
static inline bool ss_snapshot(StatsStore *s, const char *device, DeviceStats *out) {
    pthread_rwlock_rdlock(&s->lock);   // reader: shared
    bool found = false;
    for (int i = 0; i < s->used; i++) {
        if (strcmp(s->entries[i].name, device) == 0) {
            *out = s->entries[i].stats;   // copy, so the lock can be released
            found = true;
            break;
        }
    }
    pthread_rwlock_unlock(&s->lock);
    return found;
}

#endif
