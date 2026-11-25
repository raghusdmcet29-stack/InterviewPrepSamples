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

// Cut the file into 4 KB chunks
var chunks: [Data] = []
var pos = 0
while pos < data.count {
    let end = min(pos + 4096, data.count)
    chunks.append(Data(data[pos..<end]))
    pos = end
}

// Fingerprint each chunk
let fps = chunks.map { fingerprint(of: $0) }

// Build one HAVE line per chunk
let message = Data(fps.map { "HAVE \($0)\n" }.joined().utf8)

let connection = NWConnection(host: "127.0.0.1", port: 9100, using: .tcp)

var replies = ""
var waitingForHave = true
var sentCount = 0
var committing = false

// Reads replies and moves through the phases
func readAll() {
    connection.receive(minimumIncompleteLength: 1, maximumLength: 4096) { content, _, _, _ in
        if let content, let text = String(data: content, encoding: .utf8) {
            replies += text
        }
        let lines = replies.split(separator: "\n")

        if waitingForHave {
            // Phase 1: wait for one answer per chunk, then send the chunks the server needs
            guard lines.count >= fps.count else { readAll(); return }
            waitingForHave = false
            print("Server replied:\n\(replies)")

            var out = Data()
            var sent = 0
            for (i, answer) in lines.enumerated() where answer == "NEED" {
                out.append(Data("CHUNK \(fps[i]) \(chunks[i].count)\n".utf8))
                out.append(chunks[i])
                sent += 1
            }
            replies = ""

            if sent == 0 {
                print("Server already has all chunks")
                sendCommit()
                return
            }

            sentCount = sent
            print("Sending \(sent) chunk(s)")
            connection.send(content: out, completion: .contentProcessed { error in
                if let error {
                    print("Send failed: \(error)")
                    exit(1)
                }
                readAll()
            })

        } else if !committing {
            // Phase 2: wait for one OK per chunk sent, then send COMMIT
            guard lines.count >= sentCount else { readAll(); return }
            print("CHUNK replies:\n\(replies)")
            sendCommit()

        } else {
            // Phase 3: wait for the COMMIT reply, then exit
            guard lines.count >= 1 else { readAll(); return }
            print("COMMIT reply:\n\(replies)")
            connection.cancel()
            exit(0)
        }
    }
}

func sendCommit() {
    committing = true
    replies = ""
    connection.send(content: Data("COMMIT test.txt \(fps.joined(separator: " "))\n".utf8), completion: .contentProcessed { _ in })
    readAll()
}

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
            print("Sent Data")
        })
        readAll()
    }
}
connection.start(queue: .main)
dispatchMain()
