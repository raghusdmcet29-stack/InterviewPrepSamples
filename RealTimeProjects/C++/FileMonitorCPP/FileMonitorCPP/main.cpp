//
//  main.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 03/11/26.
//

#include "Watcher.hpp"
#include "Config.hpp"
#include "Database.hpp"
#include "Sync.hpp"

#include <dispatch/dispatch.h>
#include <iostream>

int main() {
    if (!openDatabase(databasePath())) {
        std::cout << "could not open database" << std::endl;
        return 1;
    }

    syncFolder(watchFolderPath());
    std::cout << "sync done" << std::endl;

    startWatching(watchFolderPath());
    dispatch_main();
}
