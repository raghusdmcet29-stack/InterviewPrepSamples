//
//  Scanner.hpp
//  FileMonitorCPP
//
//  Created by Anussha on 03/11/26.
//

#ifndef Scanner_hpp
#define Scanner_hpp

#include <stdio.h>

#pragma once

#include <string>

// Returns true if a file or folder exists at the given path.
bool fileExists(const std::string& path);

#endif /* Scanner_hpp */
