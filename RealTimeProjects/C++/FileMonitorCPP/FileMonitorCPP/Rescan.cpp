//
//  Rescan.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#include "Rescan.hpp"
#include <filesystem>

std::set<std::string> listFiles(const std::string& folder) {
    std::set<std::string> result;

    for (const auto& entry : std::filesystem::directory_iterator(folder)) {
        std::string name = entry.path().filename().string();

        // Skip hidden files such as .DS_Store
        if (!name.empty() && name[0] == '.') {
            continue;
        }

        if (entry.is_regular_file()) {
            result.insert(entry.path().string());
        }
    }

    return result;
}
