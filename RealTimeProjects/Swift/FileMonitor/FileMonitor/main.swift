//
//  main.swift
//  FileMonitor
//
//  Created by Anussha on 27/10/26.
//
import Foundation
import CoreServices

let fileManager = FileManager.default
let folderURL = fileManager.homeDirectoryForCurrentUser.appendingPathComponent("ESWatchFolder")

// Check if the folder doesn't exist, then create it
if !fileManager.fileExists(atPath: folderURL.path) {
    do {
        try fileManager.createDirectory(at: folderURL, withIntermediateDirectories: true, attributes: nil)
        print("Created directory at: \(folderURL.path)")
    } catch {
        print("Error creating directory: \(error)")
    }
}

let folder = folderURL.path
var known: Set<String> = []

func labelsFor(_ flags: FSEventStreamEventFlags) -> [String] {
    var labels: [String] = []
    if flags & UInt32(kFSEventStreamEventFlagItemCreated) != 0 { labels.append("New file") }
    if flags & UInt32(kFSEventStreamEventFlagItemRemoved) != 0 { labels.append("Removed") }
    if flags & UInt32(kFSEventStreamEventFlagItemRenamed) != 0 { labels.append("Renamed") }
    if flags & UInt32(kFSEventStreamEventFlagItemModified) != 0 ||
       flags & UInt32(kFSEventStreamEventFlagItemInodeMetaMod) != 0 {
        labels.append("Changed")
    }
    return labels
}

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
            
            print("WARNING: Events were dropped under \(path) due to: [\(reasons.joined(separator: ", "))]. A rescan is needed.")
            rescan()
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
            print("\(label): \(path)")
        }
    }
}

func rescan() {
    let current = enumerateFiles(in: folder)
    for path in current.subtracting(known).sorted() {
        print("rescan:New file: \(path)")
    }
    for path in known.subtracting(current).sorted() {
        print("rescan:Removed: \(path)")
    }
    known = current
}

// Walks the folder and all subfolders. Returns the full path of every regular file.
func enumerateFiles(in folder: String) -> Set<String> {
    var result: Set<String> = []
    guard let enumerator = FileManager.default.enumerator(atPath: folder) else { return result }
    for case let relative as String in enumerator {
        let path = folder + "/" + relative
        if path.contains(".xcodeproj") { continue }
        guard let attributes = try? FileManager.default.attributesOfItem(atPath: path),
              let type = attributes[.type] as? FileAttributeType,
              type == .typeRegular else { continue }
        result.insert(path)
    }
    return result
}

let streamFlags = UInt32(kFSEventStreamCreateFlagFileEvents | kFSEventStreamCreateFlagUseCFTypes)

guard let stream = FSEventStreamCreate(
    kCFAllocatorDefault,
    onFileEvents,
    nil,
    [folder] as CFArray,
    FSEventStreamEventId(kFSEventStreamEventIdSinceNow),
    0.5,
    streamFlags
) else {
    print("Could not create the event stream")
    exit(1)
}

FSEventStreamSetDispatchQueue(stream, DispatchQueue.main)
FSEventStreamStart(stream)

print("Watching \(folder) in real time. Press Ctrl+C to stop.")
dispatchMain()
