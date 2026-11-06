//
//  Labels.h
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#ifndef LABELS_H
#define LABELS_H

typedef enum {
    FILE_CREATED,
    FILE_MODIFIED,
    FILE_DELETED,
    FILE_RENAMED
} FileEventKind;

const char *label_for(FileEventKind kind);
#endif
