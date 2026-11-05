//
//  Database.swift
//  FileMonitorSwift
//
//  Created by Anussha on 05/11/26.
//

import Foundation
import SQLite3

let SQLITE_TRANSIENT = unsafeBitCast(-1, to: sqlite3_destructor_type.self)

func openDatabase(at path: String) -> OpaquePointer? {
    var db: OpaquePointer?
    if sqlite3_open(path, &db) != SQLITE_OK {
        print("could not open database")
        return nil
    }
    sqlite3_busy_timeout(db, 5000)
    sqlite3_exec(db, "PRAGMA journal_mode=WAL;", nil, nil, nil)
    let sql = "CREATE TABLE IF NOT EXISTS known_files (path TEXT PRIMARY KEY, added_at REAL);"
    sqlite3_exec(db, sql, nil, nil, nil)
    return db
}

func insertFile(_ db: OpaquePointer?, path: String) {
    let sql = "INSERT OR IGNORE INTO known_files (path, added_at) VALUES (?, ?);"
    var stmt: OpaquePointer?
    sqlite3_prepare_v2(db, sql, -1, &stmt, nil)
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_TRANSIENT)
    sqlite3_bind_double(stmt, 2, Date().timeIntervalSince1970)
    sqlite3_step(stmt)
    sqlite3_finalize(stmt)
}

func removeFile(_ db: OpaquePointer?, path: String) {
    let sql = "DELETE FROM known_files WHERE path = ?;"
    var stmt: OpaquePointer?
    sqlite3_prepare_v2(db, sql, -1, &stmt, nil)
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_TRANSIENT)
    sqlite3_step(stmt)
    sqlite3_finalize(stmt)
}

func allPaths(_ db: OpaquePointer?) -> [String] {
    var stmt: OpaquePointer?
    sqlite3_prepare_v2(db, "SELECT path FROM known_files;", -1, &stmt, nil)
    var result: [String] = []
    while sqlite3_step(stmt) == SQLITE_ROW {
        result.append(String(cString: sqlite3_column_text(stmt, 0)))
    }
    sqlite3_finalize(stmt)
    return result
}

func countFiles(_ db: OpaquePointer?) -> Int {
    var stmt: OpaquePointer?
    sqlite3_prepare_v2(db, "SELECT COUNT(*) FROM known_files;", -1, &stmt, nil)
    sqlite3_step(stmt)
    let count = Int(sqlite3_column_int(stmt, 0))
    sqlite3_finalize(stmt)
    return count
}

func beginTransaction(_ db: OpaquePointer?) {
    sqlite3_exec(db, "BEGIN;", nil, nil, nil)
}

func commitTransaction(_ db: OpaquePointer?) {
    sqlite3_exec(db, "COMMIT;", nil, nil, nil)
}
