//
//  Scanner.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 03/11/26.
//

#include "Scanner.hpp"

#include <filesystem>

bool fileExists(const std::string& path) {
    return std::filesystem::exists(path);
}
