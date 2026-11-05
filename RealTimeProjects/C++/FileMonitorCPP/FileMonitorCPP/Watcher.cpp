//
//  Watcher.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#include "Watcher.hpp"
#include <CoreServices/CoreServices.h>
#include <dispatch/dispatch.h>
#include <cstdio>
#include <iostream>
#include "EventHandler.hpp"
#include "Scanner.hpp"
#include "Sync.hpp"

static std::string watchedFolder;

static void eventCallback(ConstFSEventStreamRef stream, void* info,
                          size_t numEvents, void* eventPaths,
                          const FSEventStreamEventFlags eventFlags[],
                          const FSEventStreamEventId eventIds[]) {
    
    char** paths = static_cast<char**>(eventPaths);
    
    for (size_t i = 0; i < numEvents; i++) {
        
        if (eventFlags[i] & kFSEventStreamEventFlagMustScanSubDirs) {
                std::cout << "events dropped, running sync" << std::endl;
                syncFolder(watchedFolder);
                break;
            }
        
        std::string path = paths[i];
        
        
        // Skip hidden files such as .DS_Store
        std::string name = std::filesystem::path(path).filename().string();
        if (!name.empty() && name[0] == '.') {
            continue;
        }

        if (fileExists(path)) {
            handleCreated(path);
        } else {
            handleDeleted(path);
        }
        std::cout << "event: " << path << std::endl;
    }
}

void startWatching(const std::string& folder){
    watchedFolder = folder;
    CFStringRef folderRef = CFStringCreateWithCString(nullptr, folder.c_str(), kCFStringEncodingUTF8);
    CFArrayRef folders = CFArrayCreate(nullptr, (const void**)&folderRef, 1, nullptr);
    
    FSEventStreamRef stream = FSEventStreamCreate(
            nullptr,
            eventCallback,
            nullptr,
            folders,
            kFSEventStreamEventIdSinceNow,
            0.2,
            kFSEventStreamCreateFlagFileEvents);

        FSEventStreamSetDispatchQueue(stream, dispatch_queue_create("watcher", nullptr));
        FSEventStreamStart(stream);
     std::cout<<"startWatching started"<<std::endl;
    }

