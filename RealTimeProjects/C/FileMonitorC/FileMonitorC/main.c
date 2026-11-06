//
//  main.c
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <CoreServices/CoreServices.h>
#include <dispatch/dispatch.h>
#include "Labels.h"

static FileEventKind kind_from_flags(FSEventStreamEventFlags flags) {
    if (flags & kFSEventStreamEventFlagItemCreated)  return FILE_CREATED;
    if (flags & kFSEventStreamEventFlagItemRemoved)  return FILE_DELETED;
    if (flags & kFSEventStreamEventFlagItemRenamed)  return FILE_RENAMED;
    if (flags & kFSEventStreamEventFlagItemModified) return FILE_MODIFIED;
    return FILE_MODIFIED;
}

static void callback(ConstFSEventStreamRef stream, void *info,
                     size_t count, void *paths, const FSEventStreamEventFlags flags[],
                     const FSEventStreamEventId ids[]) {
    char **names = (char **)paths;
    for (size_t i = 0; i < count; i++) {
        printf("%s: %s\n", label_for(kind_from_flags(flags[i])), names[i]);
    }
}

int main(void) {
    const char *watch_dir = "/Users/anussha/ESWatchFolder";
    const char *test_file = "/Users/anussha/ESWatchFolder/fs_test.txt";

    mkdir(watch_dir, 0755);
    remove(test_file);

    CFStringRef folder = CFSTR("/Users/anussha/ESWatchFolder");
    CFArrayRef paths = CFArrayCreate(NULL, (const void **)&folder, 1, &kCFTypeArrayCallBacks);

    FSEventStreamRef stream = FSEventStreamCreate(
        NULL, callback, NULL, paths,
        kFSEventStreamEventIdSinceNow, 0.5,
        kFSEventStreamCreateFlagFileEvents);

    dispatch_queue_t queue = dispatch_queue_create("fsevents", NULL);
    FSEventStreamSetDispatchQueue(stream, queue);
    FSEventStreamStart(stream);

    printf("watching %s\n", watch_dir);

    for (int tick = 0; tick < 3; tick++) {
        sleep(1);
        if (tick == 1) {
            FILE *f = fopen(test_file, "w");
            if (f) fclose(f);
            printf("wrote %s\n", test_file);
        }
    }

    printf("still watching, press Stop in Xcode to quit\n");
    dispatch_main();
}
