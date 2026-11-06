//
//  Compare.h
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#ifndef COMPARE_H
#define COMPARE_H

#include "Snapshot.h"
#include "Labels.h"

typedef struct {
    bool changed;
    FileEventKind kind;
} Change;

Change compare_snapshots(const Snapshot *old_snap, const Snapshot *new_snap);

#endif
