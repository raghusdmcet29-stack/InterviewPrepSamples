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
