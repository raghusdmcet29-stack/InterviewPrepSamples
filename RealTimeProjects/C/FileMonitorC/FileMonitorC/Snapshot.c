//
//  Snapshot.c
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#include <string.h>
#include "Snapshot.h"
#include "FileCheck.h"

Snapshot take_snapshot(const char *path) {
    Snapshot snap;
    strncpy(snap.path, path, sizeof(snap.path) - 1);
    snap.path[sizeof(snap.path) - 1] = '\0';
    snap.exists = file_exists(path);
    snap.mtime = last_modified(path);
    return snap;
}
