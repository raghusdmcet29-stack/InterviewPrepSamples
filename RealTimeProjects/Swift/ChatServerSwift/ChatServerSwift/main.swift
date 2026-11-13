//
//  main.swift
//  ChatServerSwift
//
//  Created by Anussha on 13/11/26.
//

import Foundation

// main.swift
do {
    let server = try Server(port: 8080)
    print("listening on port 8080")
    server.run()
} catch {
    print("server failed to start: \(error)")
}

