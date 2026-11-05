//
//  main.swift
//  FileMonitorSwift
//
//  Created by Anussha on 05/11/26.
//

import Foundation

let db = openDatabase(at: config.databasePath)

rescan(db, folder: config.watchFolder)

let watcher = startWatcher(folder: config.watchFolder)
print("watching \(config.watchFolder)")
RunLoop.main.run()

