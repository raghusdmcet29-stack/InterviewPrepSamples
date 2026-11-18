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
    
    guard let storeWriter = FileHandle(forWritingAtPath: "store.dat"),
          let storeReader = FileHandle(forReadingAtPath: "store.dat") else {
        throw StoreError(message: "could not open store.dat")
    }
    defer { try? storeWriter.close() }
    defer { try? storeReader.close() }
    
    
    var shelves: [UInt64: [ChunkLocation]] = [:]   // each shelf holds locations in store.dat
    var storeEnd: UInt64 = 0                        // where the next new chunk goes
    var chunkCount = 0
    var newCount = 0
    var dupCount = 0
    
    while true{
        guard let chunk = try input.read(upToCount: 4096), !chunk.isEmpty else { break }
        chunkCount += 1
        
        let fp = fingerprint(of: chunk)
        let shelf = shelves[fp] ?? []
        
        // compare with each saved chunk on this shelf, reading its bytes from store.dat
        var matched = false
        for loc in shelf {
            if try readChunk(from: storeReader, at: loc) == chunk {
                matched = true
                break
            }
        }
        if matched {
            dupCount += 1
            print("  duplicate")
        } else {
            let loc = ChunkLocation(offset: storeEnd, length: chunk.count)
            try storeWriter.seek(toOffset: storeEnd)
            try storeWriter.write(contentsOf: chunk)
            storeEnd += UInt64(chunk.count)
            
            shelves[fp, default: []].append(loc)
            newCount += 1
            print("  new")
        }
        
    }
    print("new: \(newCount), duplicate: \(dupCount)")
    print("total chunks: \(chunkCount)")
    print("store.dat is \(storeEnd) bytes")
}
