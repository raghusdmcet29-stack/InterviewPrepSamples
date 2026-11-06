//
//  Snapshot.h
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <time.h>
#include <stdbool.h>

typedef struct {
    char path[256];
    time_t mtime;
    bool exists;
} Snapshot;

Snapshot take_snapshot(const char *path);

#endif
