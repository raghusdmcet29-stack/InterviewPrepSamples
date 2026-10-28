//
//  main.swift
//  FileMonitor
//
//  Created by Anussha on 27/10/26.
//
import Foundation
import CoreServices

let fileManager = FileManager.default

if !fileManager.fileExists(atPath: folderURL.path) {
    do {
        try fileManager.createDirectory(at: folderURL, withIntermediateDirectories: true, attributes: nil)
        print("Created directory at: \(folderURL.path)")
    } catch {
        print("Error creating directory: \(error)")
    }
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
known = enumerateFiles(in: folder)
FSEventStreamStart(stream)
openDatabase()
print("Watching \(folder) in real time. Press Ctrl+C to stop.")
dispatchMain()
