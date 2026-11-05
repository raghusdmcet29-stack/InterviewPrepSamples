//
//  Sync.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#include "Sync.hpp"
#include "Rescan.hpp"
#include "Database.hpp"
#include "EventHandler.hpp"

void syncFolder(const std::string& folder) {
    std::set<std::string> onDisk = listFiles(folder);
    std::set<std::string> inDatabase = loadAllPaths();

    beginTransaction();
    
    // On disk but not in the database: add them.
    for (const auto& path : onDisk) {
        if (inDatabase.count(path) == 0) {
            handleCreated(path);
        }
    }

    // In the database but not on disk: remove them.
    for (const auto& path : inDatabase) {
        if (onDisk.count(path) == 0) {
            handleDeleted(path);
        }
    }
    
    commitTransaction();
}
