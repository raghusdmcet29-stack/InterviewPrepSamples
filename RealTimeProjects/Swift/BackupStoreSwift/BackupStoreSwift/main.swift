//
//  main.swift
//  BackupStoreSwift
//
//  Created by Anussha on 18/11/26.
//

import Foundation

let inputPath = NSHomeDirectory() + "/test16k.txt"

do {
    try BackupController().backupFile(path: inputPath)
} catch {
    print("backup failed: \(error)")
}

do {
    let data = try RestoreController().loadRestoreData()
    print("shelves: \(data.shelves.count), order entries: \(data.order.count)")
} catch {
    print("restore load failed: \(error)")
}

do {
    try RestoreController().restoreFile(outputPath: NSHomeDirectory() + "/test16k_restored.txt")
} catch {
    print("restore failed: \(error)")
}
