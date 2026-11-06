//
//  FileEvent.c
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#include <stdio.h>
#include "FileEvent.h"

void print_event(const FileEvent *event) {
    printf("%s: %s\n", label_for(event->kind), event->path);
}
