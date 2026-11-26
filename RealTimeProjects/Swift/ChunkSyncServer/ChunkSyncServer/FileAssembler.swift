//
//  FileAssembler.swift
//  ChunkSyncServer
//
//  Created by Anussha on 26/11/26.
//

import Foundation

final class FileAssembler {
    private let partsDirectory: URL
    private let filesDirectory: URL
    private var writers: [String: FileHandle] = [:]   // one open .part file per name

    init() throws {
        let base = FileManager.default.homeDirectoryForCurrentUser
            .appendingPathComponent("ChunkSyncServer", isDirectory: true)
        partsDirectory = base.appendingPathComponent("parts", isDirectory: true)
        filesDirectory = base.appendingPathComponent("files", isDirectory: true)
        try FileManager.default.createDirectory(at: partsDirectory, withIntermediateDirectories: true)
        try FileManager.default.createDirectory(at: filesDirectory, withIntermediateDirectories: true)
    }

    private func partURL(_ name: String) -> URL {
        partsDirectory.appendingPathComponent(name + ".part")
    }

    // Starts a new empty .part file, replacing any unfinished one.
    func begin(_ name: String) throws {
        try? writers[name]?.close()
        _ = FileManager.default.createFile(atPath: partURL(name).path, contents: nil)
        writers[name] = try FileHandle(forWritingTo: partURL(name))
    }

    // Adds one chunk's bytes to the end of the .part file.
    func append(_ name: String, bytes: Data) throws {
        guard let writer = writers[name] else { throw ServerError.notStarted(name) }
        try writer.write(contentsOf: bytes)
    }

    // Closes the .part file and moves it into files/.
    func commit(_ name: String) throws {
        guard let writer = writers[name] else { throw ServerError.notStarted(name) }
        try writer.close()
        writers[name] = nil

        let finalURL = filesDirectory.appendingPathComponent(name)
        try? FileManager.default.removeItem(at: finalURL)
        try FileManager.default.moveItem(at: partURL(name), to: finalURL)
    }
}
