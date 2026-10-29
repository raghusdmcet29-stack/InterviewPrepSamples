//
//  Counts.swift
//  FileMonitor
//
//  Created by Anussha on 29/10/26.
//
import Foundation

var liveCreated = 0
var liveRemoved = 0
var rescanCreated = 0
var rescanRemoved = 0

private var lastPrinted = (0, 0, 0, 0)

func printCounts() {
    print("[\(now())] COUNT live created: \(liveCreated) removed: \(liveRemoved) | rescan created: \(rescanCreated) rescan removed: \(rescanRemoved) | known: \(known.count)")
}

// Called on eventQueue at the end of each batch.
// Prints only when a count changed since the last print.
func printIfChanged() {
    let current = (liveCreated, liveRemoved, rescanCreated, rescanRemoved)
    if current == lastPrinted { return }
    lastPrinted = current
    printCounts()
}

