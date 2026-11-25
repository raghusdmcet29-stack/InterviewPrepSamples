//
//  ChunkStore.swift
//  ChunkSyncServer
//
//  Created by Anussha on 23/11/26.
//
import Foundation

struct ChunkStore {
    private var chunks: [String: Data] = [:]

    mutating func add(_ fingerprint: String, _ bytes: Data) {
        chunks[fingerprint] = bytes
    }
    
    func contains(_ fingerprint: String) -> Bool {
        return chunks[fingerprint] != nil
    }
    
    func bytes(for fingerprint: String) -> Data? {
        return chunks[fingerprint]
    }
}
