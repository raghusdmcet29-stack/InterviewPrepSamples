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

let dbQueue = DispatchQueue(label: "filemonitor.db")
var pendingOps: [(path: String, insert: Bool)] = []
var flushScheduled = false

func openDatabase() {
    try? FileManager.default.createDirectory(atPath: dbFolder, withIntermediateDirectories: true)

    guard sqlite3_open(dbFolder + "/known.sqlite", &db) == SQLITE_OK else {
        print("[\(now())] Could not open database")
        exit(1)
    }

    let createSQL = "CREATE TABLE IF NOT EXISTS known_files (path TEXT PRIMARY KEY);"
    guard sqlite3_exec(db, createSQL, nil, nil, nil) == SQLITE_OK else {
        print("[\(now())] Could not create table")
        exit(1)
    }
    print("[\(now())] Database ready")
}

func dbInsert(_ path: String) {
    enqueueDB(path: path, insert: true)
}

func dbRemove(_ path: String) {
    enqueueDB(path: path, insert: false)
}

func enqueueDB(path: String, insert: Bool) {
    dbQueue.async {
        pendingOps.append((path: path, insert: insert))
        if !flushScheduled {
            flushScheduled = true
            dbQueue.asyncAfter(deadline: .now() + 0.2) { flushDB() }
        }
    }
}

func flushDB() {
    flushScheduled = false
    let ops = pendingOps
    pendingOps.removeAll()
    if ops.isEmpty { return }
    
    var insertStmt: OpaquePointer?
    var deleteStmt: OpaquePointer?
    
    sqlite3_prepare_v2(db, "INSERT OR IGNORE INTO known_files (path) VALUES (?);", -1, &insertStmt, nil)
    sqlite3_prepare_v2(db, "DELETE FROM known_files WHERE path = ?;", -1, &deleteStmt, nil)
    
    sqlite3_exec(db, "BEGIN TRANSACTION;", nil, nil, nil)
    for op in ops {
        let stmt = op.insert ? insertStmt : deleteStmt
        sqlite3_reset(stmt)
        sqlite3_bind_text(stmt, 1, op.path, -1, SQLITE_TRANSIENT)
        sqlite3_step(stmt)
    }
    sqlite3_exec(db, "COMMIT;", nil, nil, nil)
    
    sqlite3_finalize(insertStmt)
    sqlite3_finalize(deleteStmt)
    print("[\(now())] DB flushed \(ops.count) changes")
}
