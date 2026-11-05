//
//  Labels.swift
//  FileMonitorSwift
//
//  Created by Anussha on 05/11/26.
//

enum FileEventKind {
    case created
    case modified
    case deleted
    case renamed
}

struct FileEvent {
    let kind: FileEventKind
    let path: String
}

func printEvent(_ event: FileEvent) {
    print("\(label(for: event.kind)): \(event.path)")
}


func label(for kind: FileEventKind) -> String {
    
    switch kind {
    case .created: return "created"
    case .modified: return "modified"
    case .deleted: return "deleted"
    case .renamed: return "renamed"
    }
}
