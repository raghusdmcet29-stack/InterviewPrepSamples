//
//  main.swift
//  BackupStoreSwift
//
//  Created by Anussha on 18/11/26.
//

import Foundation

print("Backup file path: ", terminator: "")
let path = (readLine() ?? "").trimmingCharacters(in: .whitespaces)

do {
    try backupFile(path: path)
} catch {
    print("error: \(error)")
}
