//
//  FileCheck.c
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#include <sys/stat.h>
#include "FileCheck.h"

bool file_exists(const char *path) {
    struct stat info;
    return stat(path, &info) == 0;
}

time_t last_modified(const char *path) {
    struct stat info;
    if (stat(path, &info) != 0) {
        return 0;
    }
    return info.st_mtime;
}
