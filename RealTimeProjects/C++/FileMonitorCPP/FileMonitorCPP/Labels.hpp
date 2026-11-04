//
//  Labels.hpp
//  FileMonitorCPP
//
//  Created by Anussha on 03/11/26.
//

#ifndef Labels_hpp
#define Labels_hpp

#include <stdio.h>
#include <string>
#pragma once

enum class FileEventKind {
    Created,
    Modified,
    Deleted
};
std::string label(FileEventKind kind);

#endif /* Labels_hpp */
