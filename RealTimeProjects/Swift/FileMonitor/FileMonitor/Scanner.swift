//
//  Scanner.swift
//  FileMonitor
//
//  Created by Anussha on 28/10/26.
//

import Foundation

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
