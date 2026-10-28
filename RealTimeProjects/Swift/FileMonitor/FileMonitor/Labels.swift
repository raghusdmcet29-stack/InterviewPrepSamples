//
//  Labels.swift
//  FileMonitor
//
//  Created by Anussha on 28/10/26.
//

import Foundation
import CoreServices

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
