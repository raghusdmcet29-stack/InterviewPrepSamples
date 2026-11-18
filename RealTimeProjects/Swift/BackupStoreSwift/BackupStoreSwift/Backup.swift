//
//  Backup.swift
//  BackupStoreSwift
//
//  Created by Anussha on 18/11/26.
//

import Foundation

func backupFile(path: String) throws {
    let attributes = try FileManager.default.attributesOfItem(atPath: path)
    let size = attributes[.size] as! Int
    print("\(path) is \(size) bytes")
    
    let input = try FileHandle(forReadingFrom: URL(fileURLWithPath: path))
    defer { try? input.close() }
    
    var shelves: [UInt64: [Data]] = [:]   // temporary: chunk bytes still in memory
    var chunkCount = 0
    var newCount = 0
    var dupCount = 0
    
    while true{
        guard let chunk = try input.read(upToCount: 4096), !chunk.isEmpty else { break }
        chunkCount += 1
        
        let fp = fingerprint(of: chunk)
        let shelf = shelves[fp] ?? []
        if shelf.contains(chunk) {
            dupCount += 1
            print("  duplicate")
        }else {
            shelves[fp, default: []].append(chunk)
            newCount += 1
            print("  new")
        }
    }
    print("new: \(newCount), duplicate: \(dupCount)")
    print("total chunks: \(chunkCount)")
}
