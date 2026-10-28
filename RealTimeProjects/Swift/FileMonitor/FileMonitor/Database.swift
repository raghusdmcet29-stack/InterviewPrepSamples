//
//  Database.swift
//  FileMonitor
//
//  Created by Anussha on 28/10/26.
//

import Foundation
import SQLite3

let dbFolder = NSHomeDirectory() + "/FileMonitorData"
var db: OpaquePointer?
let SQLITE_TRANSIENT = unsafeBitCast(-1, to: sqlite3_destructor_type.self)

func openDatabase() {
    try? FileManager.default.createDirectory(atPath: dbFolder, withIntermediateDirectories: true)

    guard sqlite3_open(dbFolder + "/known.sqlite", &db) == SQLITE_OK else {
        print("Could not open database")
        exit(1)
    }

    let createSQL = "CREATE TABLE IF NOT EXISTS known_files (path TEXT PRIMARY KEY);"
    guard sqlite3_exec(db, createSQL, nil, nil, nil) == SQLITE_OK else {
        print("Could not create table")
        exit(1)
    }
    print("Database ready")
}

func dbInsert(_ path: String) {
    var stmt: OpaquePointer?
    let sql = "INSERT OR IGNORE INTO known_files (path) VALUES (?);"
    guard sqlite3_prepare_v2(db, sql, -1, &stmt, nil) == SQLITE_OK else {
        print("DB prepare failed")
        return
    }
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_TRANSIENT)
    sqlite3_step(stmt)
    sqlite3_finalize(stmt)
}

func dbRemove(_ path: String) {
    var stmt: OpaquePointer?
    let sql = "DELETE FROM known_files WHERE path = ?;"
    guard sqlite3_prepare_v2(db, sql, -1, &stmt, nil) == SQLITE_OK else {
        print("DB prepare failed")
        return
    }
    sqlite3_bind_text(stmt, 1, path, -1, SQLITE_TRANSIENT)
    sqlite3_step(stmt)
    sqlite3_finalize(stmt)
}
