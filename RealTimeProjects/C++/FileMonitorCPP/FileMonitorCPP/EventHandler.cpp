//
//  EventHandler.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 03/11/26.
//

#include "EventHandler.hpp"
#include <iostream>
#include <chrono>
#include "Database.hpp"

void printEvent(const FileEvent& event) {
    std::cout << label(event.kind) << ": " << event.path << "\n";
}

void handleCreated(const std::string& path) {
    double now = std::chrono::duration<double>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    insertPath(path, now);
}

void handleDeleted(const std::string& path) {
    removePath(path);
}
