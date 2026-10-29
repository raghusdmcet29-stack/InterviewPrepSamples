//
//  Config.swift
//  FileMonitor
//
//  Created by Anussha on 28/10/26.
//

import Foundation

let folderURL = FileManager.default.homeDirectoryForCurrentUser.appendingPathComponent("ESWatchFolder")
let folder = folderURL.path
let timeFormatter: DateFormatter = {
    let f = DateFormatter()
    f.dateFormat = "HH:mm:ss.SSS"
    return f
}()

func now() -> String {
    return timeFormatter.string(from: Date())
}
