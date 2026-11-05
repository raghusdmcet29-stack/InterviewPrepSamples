//
//  Rescan.swift
//  FileMonitorSwift
//
//  Created by Anussha on 05/11/26.
//

import Foundation

func listFiles(in folder: String) -> Set<String> {
    var result = Set<String>()
    let url = URL(fileURLWithPath: folder)
    guard let enumerator = FileManager.default.enumerator(
        at: url,
        includingPropertiesForKeys: [.isRegularFileKey],
        options: [.skipsHiddenFiles, .skipsSubdirectoryDescendants]
    ) else { return result }

    for case let fileURL as URL in enumerator {
        if (try? fileURL.resourceValues(forKeys: [.isRegularFileKey]))?.isRegularFile == true {
            result.insert(fileURL.path)
        }
    }
    return result
}

// Same as C++ syncFolder: compare folder with database, fix the difference in one transaction.
func rescan(_ db: OpaquePointer?, folder: String) {
    let onDisk = listFiles(in: folder)
    let inDatabase = Set(allPaths(db))

    beginTransaction(db)
    for path in onDisk where !inDatabase.contains(path) {
        insertFile(db, path: path)
    }
    for path in inDatabase where !onDisk.contains(path) {
        removeFile(db, path: path)
    }
    commitTransaction(db)
    print("rescan done, count: \(countFiles(db))")
}
