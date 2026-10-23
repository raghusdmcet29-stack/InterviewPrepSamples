//
//  main.swift
//  ActorBank
//
//  Created by Anussha on 19/10/26.
//
import Foundation

let startingBalances: [OpeningBalance] = [
    OpeningBalance(accountId: "A", balance: 100),
    OpeningBalance(accountId: "B", balance: 50),
    OpeningBalance(accountId: "F", balance: 10),
]

seedJournalIfEmpty(startingBalances, at: journalFileURL)
let startupBalances = rebuildBalances(from: journalFileURL)
print("Rebuilt A: \(startupBalances["A"] ?? 0)")
print("Rebuilt B: \(startupBalances["B"] ?? 0)")
print("Journal at: \(journalFileURL.path)")

Task {
    for opening in startingBalances {
        let balance = startupBalances[opening.accountId] ?? opening.balance
        await bank.openAccount(id: opening.accountId, balance: balance)
    }
    await bank.freeze(id: "F")

    do {
        try startServer(port: 9000)
    } catch {
        print("server failed to start: \(error)")
    }
}

dispatchMain()
