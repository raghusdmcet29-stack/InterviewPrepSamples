//
//  Config.swift
//  FileMonitorSwift
//
//  Created by Anussha on 05/11/26.
//

import Foundation

struct Config {
    let watchFolder: String
    let databasePath: String
}

let config = Config(
    watchFolder: NSHomeDirectory() + "/ESWatchFolder",
    databasePath: NSHomeDirectory() + "/FileMonitorData/known.sqlite"
)
