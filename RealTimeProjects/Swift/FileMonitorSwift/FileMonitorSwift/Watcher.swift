//
//  Watcher.swift
//  FileMonitorSwift
//
//  Created by Anussha on 05/11/26.
//
import Foundation
import CoreServices

let watcherQueue = DispatchQueue(label: "watcher")

func watcherCallback(
    streamRef: ConstFSEventStreamRef,
    clientCallBackInfo: UnsafeMutableRawPointer?,
    numEvents: Int,
    eventPaths: UnsafeMutableRawPointer,
    eventFlags: UnsafePointer<FSEventStreamEventFlags>,
    eventIds: UnsafePointer<FSEventStreamEventId>
) {
    let paths = Unmanaged<NSArray>.fromOpaque(eventPaths).takeUnretainedValue() as! [String]

    for i in 0..<numEvents {
        if eventFlags[i] & FSEventStreamEventFlags(kFSEventStreamEventFlagMustScanSubDirs) != 0 {
            print("events dropped, running sync")
            rescan(db, folder: config.watchFolder)
            return
        }

        let path = paths[i]
        let name = (path as NSString).lastPathComponent
        if name.hasPrefix(".") { continue }

        if fileExists(at: path) {
            insertFile(db, path: path)
        } else {
            removeFile(db, path: path)
        }
        print("event: \(path)")
    }
}

func startWatcher(folder: String) -> FSEventStreamRef? {
    var context = FSEventStreamContext()
    guard let stream = FSEventStreamCreate(
        nil,
        watcherCallback,
        &context,
        [folder] as CFArray,
        FSEventStreamEventId(kFSEventStreamEventIdSinceNow),
        0.2,
        UInt32(kFSEventStreamCreateFlagFileEvents | kFSEventStreamCreateFlagUseCFTypes)
    ) else { return nil }

    FSEventStreamSetDispatchQueue(stream, watcherQueue)
    FSEventStreamStart(stream)
    return stream
}
