//
//  Config.hpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#ifndef Config_hpp
#define Config_hpp
#pragma once
#include <stdio.h>
#include <string>

// Folder to watch and the SQLite file that stores known paths.
std::string watchFolderPath();
std::string databasePath();

#endif /* Config_hpp */
