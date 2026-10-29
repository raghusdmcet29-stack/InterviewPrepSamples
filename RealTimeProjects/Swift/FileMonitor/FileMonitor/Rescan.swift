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
    let current = enumerateFiles(in: folder)
    
    // `known` is only changed on eventQueue, so the result is applied there.
    rescanQueue.async {
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
        // Apply the difference instead of replacing `known`, so events
        // that arrived during the listing aren't undone.
        
        known.formUnion(newFiles)
        known.subtract(removedFiles)
        printIfChanged()
    }
}
