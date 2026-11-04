//
//  Labels.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 03/11/26.
//

#include "Labels.hpp"

std::string label(FileEventKind kind) {
    switch (kind) {
        case FileEventKind::Created: return "created";
        case FileEventKind::Modified: return "modified";
        case FileEventKind::Deleted: return "deleted";
    }
    return "unknown";
}
