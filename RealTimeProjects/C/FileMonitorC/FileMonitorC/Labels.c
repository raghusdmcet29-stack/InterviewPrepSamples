//
//  Labels.c
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#include "Labels.h"

const char *label_for(FileEventKind kind) {
    switch (kind) {
        case FILE_CREATED:  return "created";
        case FILE_MODIFIED: return "modified";
        case FILE_DELETED:  return "deleted";
        case FILE_RENAMED:  return "renamed";
    }
    return "unknown";
}
