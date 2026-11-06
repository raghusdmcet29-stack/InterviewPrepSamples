//
//  FileEvent.h
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#ifndef FileEvent_h
#define FileEvent_h

#ifndef FILE_EVENT_H
#define FILE_EVENT_H

#include "Labels.h"

typedef struct {
    FileEventKind kind;
    const char *path;
} FileEvent;

void print_event(const FileEvent *event);

#endif

#endif /* FileEvent_h */
