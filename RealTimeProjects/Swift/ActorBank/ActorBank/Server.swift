//
//  Server.swift
//  ActorBank
//
//  Created by Anussha on 20/10/26.
//

import Foundation
import Network

let serverQueue = DispatchQueue(label: "bank.server")
var activeListener: NWListener?
let bank = Bank()

func startServer(port: UInt16) throws {
    let listener = try NWListener(using: .tcp, on: NWEndpoint.Port(rawValue: port)!)
    activeListener = listener

    listener.newConnectionHandler = { connection in
        connection.start(queue: serverQueue)

        connection.receive(minimumIncompleteLength: 1, maximumLength: 1024) { data, _, _, _ in
            guard let data, let text = String(data: data, encoding: .utf8) else {
                connection.cancel()
                return
            }
            let line = text.trimmingCharacters(in: .whitespacesAndNewlines)
            print("received: \(line)")

            Task {
                let reply = await handleLine(line)
                connection.send(content: Data((reply + "\n").utf8), completion: .contentProcessed { _ in
                                    connection.cancel()
                })
            }
        }
    }

    listener.start(queue: serverQueue)
    print("listening on port \(port)")
}

func handleLine(_ line: String) async -> String {
    let firstWord = line.split(separator: " ").first.map(String.init) ?? ""
    
    let command: Command?
    switch firstWord {
    case "deposit":
        command = parseDeposit(line)
    case "transfer":
        command = parseTransfer(line)
    case "balance":
            command = parseBalance(line)
    default:
            command = nil
    }
    
    guard let cmd = command else {
        return "error: could not parse"
    }
    return await run(cmd, on: bank)
}
