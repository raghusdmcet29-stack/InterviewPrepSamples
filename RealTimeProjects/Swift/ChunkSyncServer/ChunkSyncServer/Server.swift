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

var chunkStore = ChunkStore()


// One per connection. A class is shared, so every callback updates the same buffer.
final class ConnectionBuffer {
    var lines = LineBuffer()
    var waitingFingerprint: String? = nil   // set after a valid CHUNK header
    var waitingLength = 0
}

// Sends one reply line back to the client. The "\n" marks the end of the reply.
func sendReply(_ text: String, on connection: NWConnection) {
    connection.send(content: Data((text + "\n").utf8), completion: .contentProcessed { _ in })
}

// Decides what to answer for one complete line.
func handleLine(_ line: String, _ state: ConnectionBuffer, on connection: NWConnection) {
    print("Line: \(line)")
    if isValidHaveLine(line) {
        // "HAVE <fingerprint>": the fingerprint is the second word
        let parts = line.split(separator: " ")
        let fingerprint = String(parts[1])

        if chunkStore.contains(fingerprint) {
            sendReply("HAVE_IT", on: connection)
        } else {
            sendReply("NEED", on: connection)
        }
    }else if line.hasPrefix("COMMIT ") {
        handleCommit(line, on: connection)
    }else if let header = parseChunkHeader(line) {
        // Don't reply yet. The next bytes on the connection are the chunk data.
        state.waitingFingerprint = header.fingerprint
        state.waitingLength = header.length
    }else {
        sendReply(respErr, on: connection)
    }
}

// Handles "COMMIT <filename> <fp0> <fp1> ..."
func handleCommit(_ line: String, on connection: NWConnection) {
    let parts = line.split(separator: " ").map(String.init)
    guard parts.count >= 3 else {
        sendReply(respErr, on: connection)
        return
    }
    
    let filename = parts[1]
    let fingerprints = Array(parts.dropFirst(2))
    
    var output = Data()
    for fp in fingerprints {
        guard let piece = chunkStore.bytes(for: fp) else {
            print("COMMIT missing chunk \(fp)")
            sendReply(respErr, on: connection)
            return
        }
        output.append(piece)
    }
    
    let folder = NSString(string: "~/ChunkSyncServer/files").expandingTildeInPath
    try? FileManager.default.createDirectory(atPath: folder, withIntermediateDirectories: true)
    let target = URL(fileURLWithPath: folder).appendingPathComponent(filename)
    do {
        try output.write(to: target)
        print("Wrote \(output.count) bytes to \(target.path)")
        sendReply(respOK, on: connection)
    }catch {
        print("Write failed: \(error)")
        sendReply(respErr, on: connection)
    }
}

// Splits a valid "CHUNK <fp> <length>" line into its parts.
func parseChunkHeader(_ line: String) -> (fingerprint: String, length: Int)? {
    let parts = line.split(separator: " ")
    guard isValidChunkHeader(line), let length = Int(parts[2]) else {
        return nil
    }
    return (String(parts[1]), length)
}

// True only if the bytes hash to the fingerprint the client claimed.
func bytesMatchFingerprint(_ bytes: Data, _ claimed: String) -> Bool {
    return fingerprint(of: bytes) == claimed
}

// Checks "CHUNK <64-hex fingerprint> <length>" with length from 1 to 4096.
func isValidChunkHeader(_ line: String) -> Bool {
    let parts = line.split(separator: " ")
    guard parts.count == 3, parts[0] == "CHUNK" else {
        return false
    }
    
    let fingerprint = String(parts[1])
    guard fingerprint.count == 64, fingerprint.allSatisfy({ $0.isHexDigit }) else {
        return false
    }
    
    guard let length = Int(parts[2]), length >= 1, length <= 4096 else {
        return false
    }
    return true
}

// Runs after exactly `length` bytes have arrived for a CHUNK.
func handleChunkBytes(_ bytes: Data, fingerprint: String, on connection: NWConnection) {
    if bytesMatchFingerprint(bytes, fingerprint) {
        chunkStore.add(fingerprint, bytes)
        sendReply(respOK, on: connection)
    }else {
        sendReply(respErr, on: connection)
    }
}

func receiveLoop(_ connection: NWConnection, _ state: ConnectionBuffer) {
    connection.receive(minimumIncompleteLength: 1, maximumLength: 1024) { data, _, isComplete, error in
        if let data = data, !data.isEmpty {
            state.lines.append(data)
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
    
    // Works through everything in the buffer: lines normally, raw bytes while a CHUNK is waiting.
    func processBuffered(_ state: ConnectionBuffer, _ connection: NWConnection) {
        while true {
            if let fingerprint = state.waitingFingerprint {
                guard let bytes = state.lines.nextBytes(count: state.waitingLength) else {
                    return   // not all the bytes have arrived yet
                }
                state.waitingFingerprint = nil
                handleChunkBytes(bytes, fingerprint: fingerprint, on: connection)
            }else {
                guard let line = state.lines.nextLine() else {
                    return   // no complete line yet
                }
                handleLine(line, state, on: connection)
            }
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

