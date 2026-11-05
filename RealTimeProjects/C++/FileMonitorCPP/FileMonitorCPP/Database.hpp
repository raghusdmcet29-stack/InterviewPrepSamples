//
//  Database.hpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#ifndef Database_hpp
#define Database_hpp
#pragma once
#pragma once

#include <string>
#include <set>
// Opens the database file and creates the known_files table if it doesn't exist.
bool openDatabase(const std::string& path);

// Closes the database.
void closeDatabase();

// Adds a path to known_files. Returns true on success.
bool insertPath(const std::string& path, double addedAt);

// Removes a path from known_files. Returns true on success.
bool removePath(const std::string& path);

// Returns how many rows are in known_files.
int countPaths();

// Returns every path currently stored in known_files.
std::set<std::string> loadAllPaths();

// Starts a transaction. Writes are held until commitTransaction.
bool beginTransaction();

// Saves all writes made since beginTransaction.
bool commitTransaction();

#endif /* Database_hpp */
