//
//  ChunkStore.swift
//  ChunkSyncServer
//
//  Created by Anussha on 23/11/26.
//
import Foundation

final class ChunkStore {
    private let directory: URL

    init() throws {
        directory = FileManager.default.homeDirectoryForCurrentUser
            .appendingPathComponent("ChunkSyncServer/chunks", isDirectory: true)
        try FileManager.default.createDirectory(at: directory, withIntermediateDirectories: true)
    }

    private func url(for fingerprint: String) -> URL {
        directory.appendingPathComponent(fingerprint)
    }

    func contains(_ fingerprint: String) -> Bool {
        FileManager.default.fileExists(atPath: url(for: fingerprint).path)
    }

    // Reads one chunk (at most 4 KB) from disk.
    func read(_ fingerprint: String) -> Data? {
        try? Data(contentsOf: url(for: fingerprint))
    }

    // Saves a chunk atomically, so a crash can't leave a half-written file.
    func add(_ fingerprint: String, bytes: Data) throws {
        guard !contains(fingerprint) else { return }
        try bytes.write(to: url(for: fingerprint), options: .atomic)
    }
}
