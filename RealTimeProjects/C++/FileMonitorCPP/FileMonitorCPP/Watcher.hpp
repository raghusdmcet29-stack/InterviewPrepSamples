//
//  Watcher.hpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#ifndef Watcher_hpp
#define Watcher_hpp
#pragma once

#include <string>
#include <stdio.h>
#include <filesystem>

// Starts watching a folder and prints the path of every change.
void startWatching(const std::string& folder);

#endif /* Watcher_hpp */
