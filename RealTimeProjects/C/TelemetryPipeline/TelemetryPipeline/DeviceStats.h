//
//  DeviceStats.h
//  TelemetryPipeline
//
//  Created by Anussha on 15/10/26.
//

#ifndef DEVICE_STATS_H
#define DEVICE_STATS_H

#include <float.h>

typedef struct {
    int count;
    double min;
    double max;
    double sum;
} DeviceStats;

static inline void ds_init(DeviceStats *s) {
    s->count = 0;
    s->min = DBL_MAX;
    s->max = -DBL_MAX;
    s->sum = 0;
}

static inline void ds_add(DeviceStats *s, double value) {
    s->count++;
    if (value < s->min) s->min = value;
    if (value > s->max) s->max = value;
    s->sum += value;
}

static inline double ds_average(const DeviceStats *s) {
    return s->count == 0 ? 0 : s->sum / s->count;
}

#endif
