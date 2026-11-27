//
//  main.swift
//  ChunkSyncClient
//
//  Created by Anussha on 25/11/26.
//

import Foundation
import Network

enum ClientError: Error {
    case connectionClosed
    case unexpectedReply(step: String, reply: String)
}

final class SyncClient {
    private let connection: NWConnection
    private var buffer = Data()

    init(port: UInt16) {
        connection = NWConnection(host: "127.0.0.1",
                                  port: NWEndpoint.Port(rawValue: port)!,
                                  using: .tcp)
    }

    func connect() async throws {
        try await withCheckedThrowingContinuation { (continuation: CheckedContinuation<Void, Error>) in
            connection.stateUpdateHandler = { state in
                switch state {
                case .ready:
                    self.connection.stateUpdateHandler = nil
                    continuation.resume()
                case .failed(let error):
                    self.connection.stateUpdateHandler = nil
                    continuation.resume(throwing: error)
                default:
                    break
                }
            }
            connection.start(queue: .main)
        }
    }

    func send(_ data: Data) async throws {
        try await withCheckedThrowingContinuation { (continuation: CheckedContinuation<Void, Error>) in
            connection.send(content: data, completion: .contentProcessed { error in
                if let error {
                    continuation.resume(throwing: error)
                } else {
                    continuation.resume()
                }
            })
        }
    }

    func sendLine(_ line: String) async throws {
        try await send(Data((line + "\n").utf8))
    }

    // Returns the next full line from the server. Replies are short, so the buffer stays small.
    func readLine() async throws -> String {
        while true {
            if let newline = buffer.firstIndex(of: 0x0A) {
                let line = String(decoding: buffer[..<newline], as: UTF8.self)
                buffer = Data(buffer[(newline + 1)...])
                return line
            }
            buffer.append(try await receive())
        }
    }

    private func receive() async throws -> Data {
        try await withCheckedThrowingContinuation { (continuation: CheckedContinuation<Data, Error>) in
            connection.receive(minimumIncompleteLength: 1, maximumLength: 65536) { content, _, isComplete, error in
                if let error {
                    continuation.resume(throwing: error)
                } else if let content, !content.isEmpty {
                    continuation.resume(returning: content)
                } else if isComplete {
                    continuation.resume(throwing: ClientError.connectionClosed)
                } else {
                    continuation.resume(returning: Data())
                }
            }
        }
    }

    func expectOK(_ step: String) async throws {
        let reply = try await readLine()
        guard reply == respOK else {
            throw ClientError.unexpectedReply(step: step, reply: reply)
        }
    }

    
    // Sends one file, one chunk at a time. Only one piece is in memory.
    func sync(filePath: String, remoteName: String) async throws {
        try await sendLine("BEGIN \(remoteName)")
        try await expectOK("BEGIN")
        print("[client begin] \(remoteName)")

        let file = try FileHandle(forReadingFrom: URL(fileURLWithPath: filePath))
        defer { try? file.close() }

        var total = 0
        var sent = 0
        while let piece = try file.read(upToCount: chunkSize), !piece.isEmpty {
            let fp = fingerprint(of: piece)
            print("[piece] read \(piece.count) bytes, fp \(fp.prefix(8))…")

            print("[client have] \(fp.prefix(8))…")
            try await sendLine("HAVE \(fp)")
            let answer = try await readLine()
            print("[client answer] \(answer)")

            if answer == respNeed {
                print("[client chunk] sending \(piece.count) bytes")
                var message = Data("CHUNK \(fp) \(piece.count)\n".utf8)
                message.append(piece)
                try await send(message)
                try await expectOK("CHUNK")
                sent += 1
            } else if answer != respHaveIt {
                throw ClientError.unexpectedReply(step: "HAVE", reply: answer)
            }

            print("[client append] \(fp.prefix(8))…")
            try await sendLine("APPEND \(remoteName) \(fp)")
            try await expectOK("APPEND")
            total += 1
        }

        print("[client commit]")
        try await sendLine("COMMIT \(remoteName)")
        try await expectOK("COMMIT")
        print("Chunks: \(total), sent: \(sent)")
    }
}

let testFile = NSString(string: "~/Untitled_restored.txt").expandingTildeInPath
let client = SyncClient(port: serverPort)

Task {
    do {
        try await client.connect()
        print("Connected to server on port \(serverPort)")
        try await client.sync(filePath: testFile, remoteName: "test.txt")
        print("Sync finished")
        exit(0)
    } catch {
        print("Sync failed: \(error)")
        exit(1)
    }
}

dispatchMain()
