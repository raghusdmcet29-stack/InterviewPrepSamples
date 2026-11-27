//
//  Backup.hpp
//  BackupStoreCPP
//
//  Created by Anussha on 18/11/26.
//

#ifndef Backup_hpp
#define Backup_hpp

#pragma once
#include <string>

// backs up the file at path into store.dat, store.idx and store.cat
int backupFile(const std::string &path);

#endif /* Backup_hpp */
