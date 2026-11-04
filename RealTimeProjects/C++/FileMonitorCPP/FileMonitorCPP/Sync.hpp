//
//  Sync.hpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#ifndef Sync_hpp
#define Sync_hpp
#pragma once
#include <stdio.h>
#include <string>

// Makes the database match the files currently in the folder.
void syncFolder(const std::string& folder);

#endif /* Sync_hpp */
