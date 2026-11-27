//
//  Restore.hpp
//  BackupStoreCPP
//
//  Created by Anussha on 18/11/26.
//

#ifndef Restore_hpp
#define Restore_hpp

#pragma once
#include <string>

// rebuilds the backed-up file into destFolder as <name>_restored.txt
int restoreFile(const std::string &destFolder, const std::string &originalPath);

#endif /* Restore_hpp */
