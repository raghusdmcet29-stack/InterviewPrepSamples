//
//  ChunkStore.swift
//  ChunkSyncServer
//
//  Created by Anussha on 23/11/26.
//

struct ChunkStore {
    private var known: Set<String> = []

    mutating func add(_ fingerprint: String) {
        known.insert(fingerprint)
    }

    func contains(_ fingerprint: String) -> Bool {
        return known.contains(fingerprint)
    }
}
