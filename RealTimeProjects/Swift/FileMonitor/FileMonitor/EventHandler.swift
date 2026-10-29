//
//  EventHandler.swift
//  FileMonitor
//
//  Created by Anussha on 28/10/26.
//

import Foundation
import CoreServices

let eventQueue = DispatchQueue(label: "filemonitor.events")

func onFileEvents(
    _ stream: ConstFSEventStreamRef,
    _ info: UnsafeMutableRawPointer?,
    _ numEvents: Int,
    _ eventPaths: UnsafeMutableRawPointer,
    _ eventFlags: UnsafePointer<FSEventStreamEventFlags>,
    _ eventIds: UnsafePointer<FSEventStreamEventId>
) {
    let paths = unsafeBitCast(eventPaths, to: NSArray.self) as! [String]
    var seen = Set<String>()
    var needsRescan = false

    for i in 0..<numEvents {
        let path = paths[i]
        let flags = eventFlags[i]

        let mustScan = flags & UInt32(kFSEventStreamEventFlagMustScanSubDirs) != 0
        let userDropped = flags & UInt32(kFSEventStreamEventFlagUserDropped) != 0
        let kernelDropped = flags & UInt32(kFSEventStreamEventFlagKernelDropped) != 0

        if mustScan || userDropped || kernelDropped {
            var reasons: [String] = []
            if mustScan { reasons.append("MustScanSubDirs") }
            if userDropped { reasons.append("UserDropped") }
            if kernelDropped { reasons.append("KernelDropped") }

            print("[\(now())] WARNING: Events were dropped under \(path) due to: [\(reasons.joined(separator: ", "))]. A rescan is needed.")
            needsRescan = true
            continue
        }

        // Ignore Xcode's own files inside the project bundle
        if path.contains(".xcodeproj") { continue }

        let isFile = flags & UInt32(kFSEventStreamEventFlagItemIsFile) != 0
        if !isFile { continue }

        for label in labelsFor(flags) {
            let key = "\(label)|\(path)"
            if seen.contains(key) { continue }
            seen.insert(key)
            if label == "New file" { known.insert(path); dbInsert(path) }
            if label == "Removed" { known.remove(path); dbRemove(path) }
            print("[\(now())] \(label): \(path)")
        }
    }

    if needsRescan {
        pendingRescan?.cancel()
        let work = DispatchWorkItem { rescan() }
        pendingRescan = work
        eventQueue.asyncAfter(deadline: .now() + 1.0, execute: work)
    }
}
