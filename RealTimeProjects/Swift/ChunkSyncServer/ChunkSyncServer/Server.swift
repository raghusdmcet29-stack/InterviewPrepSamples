//
//  Server.swift
//  ChunkSyncServer
//
//  Created by Anussha on 23/11/26.
//
// Server.swift
// Network side: listens for clients and reads what they send.

import Foundation
import Network

enum ServerError: Error {
    case hashMismatch
    case unknownChunk(String)
    case notStarted(String)
}

var chunkStore = try! ChunkStore()
var assembler = try! FileAssembler()

struct PendingChunk {
    let fingerprint: String
    let length: Int
}

// One per connection: buffered input plus a CHUNK header waiting for its bytes.
final class ConnectionBuffer {
    var lines = LineBuffer()
    var pendingChunk: PendingChunk? = nil
}

func sendReply(_ text: String, on connection: NWConnection) {
    connection.send(content: Data((text + "\n").utf8), completion: .contentProcessed { _ in })
}

// Runs the work and replies OK, or ERR if it throws.
func reply(on connection: NWConnection, _ work: () throws -> Void) {
    do {
        try work()
        sendReply(respOK, on: connection)
    } catch {
        print("Error: \(error)")
        sendReply(respErr, on: connection)
    }
}

func handleLine(_ line: String, _ state: ConnectionBuffer, on connection: NWConnection) {
    print("Line: \(line)")
    guard let command = parseCommand(line) else {
        sendReply(respErr, on: connection)
        return
    }
    print("[cmd] parsed: \(command)")
    switch command {
    case .have(let fingerprint):
        print("[have] \(fingerprint.prefix(8))…")
        sendReply(chunkStore.contains(fingerprint) ? respHaveIt : respNeed, on: connection)

    case .chunk(let fingerprint, let length):
        print("[chunk-header] \(fingerprint.prefix(8))… length \(length), waiting for bytes")
        // No reply yet. The next bytes on the connection are the chunk.
        state.pendingChunk = PendingChunk(fingerprint: fingerprint, length: length)

    case .begin(let name):
        print("[begin] \(name)")
        reply(on: connection) { try assembler.begin(name) }

    case .append(let name, let fingerprint):
        print("[append] \(name) <- \(fingerprint.prefix(8))…")
        reply(on: connection) {
            guard let bytes = chunkStore.read(fingerprint) else {
                throw ServerError.unknownChunk(fingerprint)
            }
            try assembler.append(name, bytes: bytes)
        }

    case .commit(let name):
        print("[commit] \(name)")
        reply(on: connection) { try assembler.commit(name) }
    }
}

func handleChunkBytes(_ bytes: Data, claimed: String, on connection: NWConnection) {
    print("[chunk] verifying \(claimed.prefix(8))…")
    reply(on: connection) {
        guard fingerprint(of: bytes) == claimed else { throw ServerError.hashMismatch }
        try chunkStore.add(claimed, bytes: bytes)
    }
}

// Handles everything in the buffer: lines normally, raw bytes while a CHUNK is pending.
func processBuffered(_ state: ConnectionBuffer, _ connection: NWConnection) {
    while true {
        print("[buffer] pendingChunk: \(state.pendingChunk.map { "\($0.length) bytes" } ?? "none")")
        if let pending = state.pendingChunk {
            guard let bytes = state.lines.nextBytes(count: pending.length) else {
                return   // not all the bytes have arrived yet
            }
            print("[chunk] got \(bytes.count) raw bytes")
            state.pendingChunk = nil
            handleChunkBytes(bytes, claimed: pending.fingerprint, on: connection)
        } else {
            guard let line = state.lines.nextLine() else {
                return   // no complete line yet
            }
            handleLine(line, state, on: connection)
        }
    }
}

func receiveLoop(_ connection: NWConnection, _ state: ConnectionBuffer) {
    connection.receive(minimumIncompleteLength: 1, maximumLength: 65536) { data, _, isComplete, error in
        if let data, !data.isEmpty {
            state.lines.append(data)
            print("[recv] \(data.count) bytes arrived")
            processBuffered(state, connection)
        }
        if isComplete || error != nil {
            print("Client disconnected")
            connection.cancel()
            return
        }
        receiveLoop(connection, state)
    }
}

func startServer() {
    let listener = try! NWListener(using: .tcp, on: NWEndpoint.Port(rawValue: serverPort)!)

    listener.stateUpdateHandler = { state in
        print("Listener state: \(state)")
    }

    listener.newConnectionHandler = { connection in
        print("Client connected")
        connection.start(queue: .main)
        receiveLoop(connection, ConnectionBuffer())
    }

    listener.start(queue: .main)
    print("Waiting for clients on port \(serverPort)...")
    dispatchMain()
}
