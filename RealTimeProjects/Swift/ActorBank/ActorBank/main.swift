//
//  main.swift
//  ActorBank
//
//  Created by Anussha on 19/10/26.
//

import Foundation

Task {
    _ = await bank.openAccount(id: "A", balance: 70)
    _ = await bank.openAccount(id: "F", balance: 10)
    _ = await bank.openAccount(id: "B", balance: 50)
    if let frozen = await bank.account(withId: "F") {
        await frozen.freeze()
    }

    do {
        try startServer(port: 9000)
    } catch {
        print("server failed to start: \(error)")
    }
}

dispatchMain()
