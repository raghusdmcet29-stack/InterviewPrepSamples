//
//  Rescan.hpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#ifndef Rescan_hpp
#define Rescan_hpp
#pragma once

#include <set>
#include <string>
#include <stdio.h>

// Returns the full path of every regular file directly inside the folder.
std::set<std::string> listFiles(const std::string& folder);

#endif /* Rescan_hpp */
