//
//  main.swift
//  ChunkSyncClient
//
//  Created by Anussha on 25/11/26.
//

import Foundation
import Network

let path = NSString(string: "~/test.txt").expandingTildeInPath
let data = try Data(contentsOf: URL(fileURLWithPath: path))
print("File size: \(data.count) bytes")

let chunkSize1 = 4096
var offset = 0
var index = 0
while offset < data.count {
    let end = min(offset + chunkSize1, data.count)
    let piece = data[offset..<end]
    print("Chunk \(index): \(piece.count) bytes")
    let fp = fingerprint(of: Data(piece))
    print("  fingerprint: \(fp.prefix(16))...")
    offset = end
    index += 1
}

let chunk0 = Data(data[0..<4096])
let chunk1 = Data(data[4096..<8192])
let chunk2 = Data(data[8192..<data.count])
let fps = [fingerprint(of: chunk0), fingerprint(of: chunk1), fingerprint(of: chunk2)]

let message = Data("COMMIT test.txt \(fps.joined(separator: " "))\n".utf8)

var replyBuffer = ""

func readReply(_ connection: NWConnection) {
    connection.receive(minimumIncompleteLength: 1, maximumLength: 4096) { content, _, isComplete, error in
        if let content, let text = String(data: content, encoding: .utf8) {
            replyBuffer += text
        }
        if replyBuffer.contains("\n") || isComplete || error != nil {
            print("Server replied:\n\(replyBuffer)")
            if let error {
                print("Receive failed: \(error)")
            }
            connection.cancel()
            exit(0)
        } else {
            readReply(connection)
        }
    }
}

let connection = NWConnection(host: "127.0.0.1", port: 9100, using: .tcp)
connection.stateUpdateHandler = { state in
    print("connection state: \(state)")
    if case .ready = state {
        print("Connected to server on port 9100")
        connection.send(content: message, completion: .contentProcessed { error in
            if let error {
                print("Send failed: \(error)")
                connection.cancel()
                exit(1)
            }
            print("Sent COMMIT")
        })
        readReply(connection)
    }
}
connection.start(queue: .main)
dispatchMain()
