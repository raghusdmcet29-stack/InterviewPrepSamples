//
//  Config.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#include "Config.hpp"
#include <cstdlib>

static std::string homeDirectory(){
    const char* home = std::getenv("HOME");
    return home ? std::string(home) : std::string();
}

std::string watchFolderPath(){
    return homeDirectory() + "/ESWatchFolder";
}

std::string databasePath(){
    return homeDirectory() +  "/FileMonitorData/known.sqlite";
}
