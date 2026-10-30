//
//  Rescan.swift
//  FileMonitor
//
//  Created by Anussha on 28/10/26.
//

import Foundation

var known: Set<String> = []
var pendingRescan: DispatchWorkItem?
let rescanQueue = DispatchQueue(label: "filemonitor.rescan")

// Called on eventQueue. The slow folder listing runs on rescanQueue,
// so the event queue keeps handling new events while it runs.
func rescan() {
    print("[\(now())] rescan called")

    eventQueue.async {
        // Listing and comparison happen together on eventQueue, so no event
        // can run between them and make the snapshot stale.
        let current = enumerateFiles(in: folder)
        let newFiles = current.subtracting(known).sorted()
        let removedFiles = known.subtracting(current).sorted()

        for path in newFiles {
           // print("[\(now())] rescan:New_file: \(path)")
            dbInsert(path)
        }

        for path in removedFiles {
           // print("[\(now())] rescan:Removed_file: \(path)")
            dbRemove(path)
        }

        rescanCreated += newFiles.count
        rescanRemoved += removedFiles.count

        known.formUnion(newFiles)
        known.subtract(removedFiles)
        printIfChanged()
    }
}
