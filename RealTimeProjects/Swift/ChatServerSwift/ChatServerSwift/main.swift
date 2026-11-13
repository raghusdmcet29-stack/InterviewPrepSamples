//
//  main.swift
//  ChatServerSwift
//
//  Created by Anussha on 13/11/26.
//

import Foundation

signal(SIGPIPE, SIG_IGN)   // a write to a closed client returns an error instead of crashing the server
// main.swift
do {
    let server = try Server(port: 8080)
    print("listening on port 8080")
    server.run()
} catch {
    print("server failed to start: \(error)")
}

