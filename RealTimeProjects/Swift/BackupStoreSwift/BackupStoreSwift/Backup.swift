//
//  Backup.swift
//  BackupStoreSwift
//
//  Created by Anussha on 18/11/26.
//

import Foundation

struct BackupController {
    let storeFolder: String

    init(storeFolder: String = ".") {
        self.storeFolder = storeFolder
    }

    func backupFile(path: String) throws {
        let attributes = try FileManager.default.attributesOfItem(atPath: path)
        let size = attributes[.size] as! Int
        print("\(path) is \(size) bytes")

        let input = try FileHandle(forReadingFrom: URL(fileURLWithPath: path))
        defer { try? input.close() }

        guard let storeWriter = FileHandle(forWritingAtPath: storeFolder + "/store.dat"),
              let storeReader = FileHandle(forReadingAtPath: storeFolder + "/store.dat") else {
            throw StoreError(message: "could not open store.dat")
        }
        defer { try? storeWriter.close() }
        defer { try? storeReader.close() }

        var shelves: [UInt64: [ChunkLocation]] = [:]   // each shelf holds locations in store.dat
        var storeEnd: UInt64 = 0                        // where the next new chunk goes
        var chunkCount = 0
        var newCount = 0
        var dupCount = 0
        var order: [(UInt64, Int)] = []                 // reading list: (fingerprint, position on shelf)

        while true {
            guard let chunk = try input.read(upToCount: 4096), !chunk.isEmpty else { break }
            chunkCount += 1

            let fp = fingerprint(of: chunk)
            let shelf = shelves[fp] ?? []

            // compare with each saved chunk on this shelf, reading its bytes from store.dat
            var position = shelf.count   // a new chunk goes at the end of its shelf
            for (p, loc) in shelf.enumerated() {
                if try readChunk(from: storeReader, at: loc) == chunk {
                    position = p
                    break
                }
            }

            if position < shelf.count {
                dupCount += 1
                order.append((fp, position))
                print("  duplicate")
            } else {
                let loc = ChunkLocation(offset: storeEnd, length: chunk.count)
                try storeWriter.seek(toOffset: storeEnd)
                try storeWriter.write(contentsOf: chunk)
                storeEnd += UInt64(chunk.count)

                shelves[fp, default: []].append(loc)
                order.append((fp, shelf.count))
                newCount += 1
                print("  new")
            }
        }
        print("new: \(newCount), duplicate: \(dupCount)")
        print("total chunks: \(chunkCount)")
        print("store.dat is \(storeEnd) bytes")

        // save the reading list
        var idxText = ""
        for (fp, pos) in order {
            idxText += "\(fp) \(pos)\n"
        }
        try idxText.write(toFile: storeFolder + "/store.idx", atomically: true, encoding: .utf8)

        // save the catalog
        var catText = ""
        for (fp, locs) in shelves {
            for loc in locs {
                catText += "\(fp) \(loc.offset) \(loc.length)\n"
            }
        }
        try catText.write(toFile: storeFolder + "/store.cat", atomically: true, encoding: .utf8)
        print("store.idx and store.cat written")
    }
}
