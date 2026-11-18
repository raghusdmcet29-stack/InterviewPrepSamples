//
//  ChunkStore.swift
//  BackupStoreSwift
//
//  Created by Anussha on 18/11/26.
//

import Foundation

// Fingerprint: a short number calculated from a chunk's bytes.
// Uses the 64-bit FNV-1a algorithm, which runs through the bytes one at a time.
// The same bytes always give the same number, across runs and across machines,
// so chunks can be recognized later when the program restores.
// Different chunks can occasionally share a fingerprint, so duplicates are
// confirmed by comparing the actual bytes.
func fingerprint(of data: Data) -> UInt64 {
    var hash: UInt64 = 0xcbf29ce484222325
    for byte in data {
        hash ^= UInt64(byte)
        hash = hash &* 0x100000001b3
    }
    return hash
}

struct StoreError: Error {
    let message: String
}

struct ChunkLocation {
    let offset: UInt64   // where the chunk starts in store.dat
    let length: Int      // how many bytes it takes
}

func readChunk(from handle: FileHandle, at loc: ChunkLocation) throws -> Data {
    try handle.seek(toOffset: loc.offset)
    guard let data = try handle.read(upToCount: loc.length) else {
        throw StoreError(message: "could not read chunk at \(loc.offset)")
    }
    return data
}
