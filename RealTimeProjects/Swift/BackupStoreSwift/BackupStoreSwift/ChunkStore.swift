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
