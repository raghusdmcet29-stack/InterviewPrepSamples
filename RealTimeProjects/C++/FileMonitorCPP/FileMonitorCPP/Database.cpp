//
//  Database.cpp
//  FileMonitorCPP
//
//  Created by Anussha on 04/11/26.
//

#include "Database.hpp"
#include <sqlite3.h>


static sqlite3* db = nullptr;


bool openDatabase(const std::string& path){
    if (sqlite3_open(path.c_str(), &db) != SQLITE_OK) {
        return false;
    }
    const char* sql =  "CREATE TABLE IF NOT EXISTS known_files ("
    "path TEXT PRIMARY KEY, "
    "added_at REAL);";
    
    return sqlite3_exec(db, sql, nullptr, nullptr, nullptr) == SQLITE_OK;
}


void closeDatabase(){
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool insertPath(const std::string& path, double addedAt) {
    const char* sql = "INSERT OR IGNORE INTO known_files (path, added_at) VALUES (?, ?);";
    
    sqlite3_stmt* stmt = nullptr;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, 2, addedAt);
    
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}


bool removePath(const std::string& path) {
    const char* sql = "DELETE FROM known_files WHERE path = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }
    
    sqlite3_bind_text(stmt, 1, path.c_str(), -1, SQLITE_TRANSIENT);
    
    bool ok = sqlite3_step(stmt) == SQLITE_DONE;
    sqlite3_finalize(stmt);
    return ok;
}

int countPaths() {
    const char* sql = "SELECT COUNT(*) FROM known_files;";
    
    sqlite3_stmt* stmt = nullptr;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return -1;
    }
    
    int count = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    
    sqlite3_finalize(stmt);
    return count;
}

std::set<std::string> loadAllPaths() {
    std::set<std::string> result;

    const char* sql = "SELECT path FROM known_files;";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        return result;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char* text = sqlite3_column_text(stmt, 0);
        if (text) {
            result.insert(reinterpret_cast<const char*>(text));
        }
    }

    sqlite3_finalize(stmt);
    return result;
}
