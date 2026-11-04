//
//  EventHandler.hpp
//  FileMonitorCPP
//
//  Created by Anussha on 03/11/26.
//

#ifndef EventHandler_hpp
#define EventHandler_hpp

#include <stdio.h>
#pragma once

#include <string>
#include "Labels.hpp"

struct FileEvent {
    FileEventKind kind;
    std::string path;
};
// Records a newly created file in the database.
void handleCreated(const std::string& path);

// Removes a deleted file from the database.
void handleDeleted(const std::string& path);

#endif /* EventHandler_hpp */
