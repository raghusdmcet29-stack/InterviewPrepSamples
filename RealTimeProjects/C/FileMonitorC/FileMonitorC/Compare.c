//
//  Compare.c
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#include "Compare.h"

Change compare_snapshots(const Snapshot *old_snap, const Snapshot *new_snap) {
    Change c = { false, FILE_MODIFIED };

    if (!old_snap->exists && new_snap->exists) {
        c.changed = true;
        c.kind = FILE_CREATED;
    } else if (old_snap->exists && !new_snap->exists) {
        c.changed = true;
        c.kind = FILE_DELETED;
    } else if (old_snap->exists && new_snap->exists &&
               old_snap->mtime != new_snap->mtime) {
        c.changed = true;
        c.kind = FILE_MODIFIED;
    }

    return c;
}
