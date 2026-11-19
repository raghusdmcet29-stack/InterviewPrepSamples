//
//  Restore.swift
//  BackupStoreSwift
//
//  Created by Anussha on 19/11/26.
//

import Foundation

struct RestoreController {
    let storeFolder: String
    
    init(storeFolder: String = NSHomeDirectory()) {
           self.storeFolder = storeFolder
       }
    
    // R1: load store.cat and store.idx into memory
    func loadRestoreData() throws -> (shelves: [UInt64: [ChunkLocation]], order: [(UInt64, Int)]) {
        var shelves: [UInt64: [ChunkLocation]] = [:]
        let catText = try String(contentsOfFile: storeFolder + "/store.cat", encoding: .utf8)
        for line in catText.split(separator: "\n") {
            let parts = line.split(separator: " ")
            guard parts.count == 3,
                  let fp = UInt64(parts[0]),
                  let offset = UInt64(parts[1]),
                  let length = Int(parts[2]) else {
                throw StoreError(message: "bad line in store.cat: \(line)")
            }
            shelves[fp, default: []].append(ChunkLocation(offset: offset, length: length))
        }
        
        var order: [(UInt64, Int)] = []
        let idxText = try String(contentsOfFile: storeFolder + "/store.idx", encoding: .utf8)
        for line in idxText.split(separator: "\n") {
            let parts = line.split(separator: " ")
            guard parts.count == 2,
                  let fp = UInt64(parts[0]),
                  let pos = Int(parts[1]) else {
                throw StoreError(message: "bad line in store.idx: \(line)")
            }
            order.append((fp, pos))
        }
        return (shelves, order)
    }
    
    // R2: rebuild the original file from store.dat using the reading list
    func restoreFile(outputPath: String) throws {
        let data = try loadRestoreData()
        
        guard let storeReader = FileHandle(forReadingAtPath: storeFolder + "/store.dat") else {
            throw StoreError(message: "could not open store.dat")
        }
        defer { try? storeReader.close() }
        
        FileManager.default.createFile(atPath: outputPath, contents: nil)
        guard let output = FileHandle(forWritingAtPath: outputPath) else {
            throw StoreError(message: "could not create \(outputPath)")
        }
        defer { try? output.close() }
        
        for (fp, pos) in data.order {
            guard let shelf = data.shelves[fp], pos < shelf.count else {
                throw StoreError(message: "missing chunk for fingerprint \(fp), position \(pos)")
            }
            let chunk = try readChunk(from: storeReader, at: shelf[pos])
            try output.write(contentsOf: chunk)
        }
        print("restored \(data.order.count) chunks to \(outputPath)")
    }
}
