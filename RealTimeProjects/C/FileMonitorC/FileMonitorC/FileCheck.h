//
//  FileCheck.h
//  FileMonitorC
//
//  Created by Anussha on 06/11/26.
//

#ifndef FILE_CHECK_H
#define FILE_CHECK_H
#include <time.h>
#include <stdbool.h>

bool file_exists(const char *path);
time_t last_modified(const char *path);

#endif
