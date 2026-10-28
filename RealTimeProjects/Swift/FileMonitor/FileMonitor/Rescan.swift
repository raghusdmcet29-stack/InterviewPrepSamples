//
//  Rescan.swift
//  FileMonitor
//
//  Created by Anussha on 28/10/26.
//

import Foundation

var known: Set<String> = []
var pendingRescan: DispatchWorkItem?

func rescan() {
    print("rescan called")

    let current = enumerateFiles(in: folder)
    for path in current.subtracting(known).sorted() {
        print("rescan:New file: \(path)")
        dbInsert(path)
    }
    for path in known.subtracting(current).sorted() {
        print("rescan:Removed: \(path)")
        dbRemove(path)
    }
    known = current
}
