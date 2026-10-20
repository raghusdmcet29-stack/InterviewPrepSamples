//
//  main.swift
//  ActorBank
//
//  Created by Anussha on 19/10/26.
//
import Foundation

let folder = journalFileURL.deletingLastPathComponent()
try? FileManager.default.createDirectory(at: folder, withIntermediateDirectories: true)

if !FileManager.default.fileExists(atPath: journalFileURL.path) {
    let entries = [
        JournalEntry(sequence: 1, accountId: "A", amount: 100),
        JournalEntry(sequence: 2, accountId: "A", amount: -30),
        JournalEntry(sequence: 3, accountId: "B", amount: 50)
    ]
    let text = entries.map { $0.line }.joined(separator: "\n") + "\n"
    _ = try? text.write(to: journalFileURL, atomically: true, encoding: .utf8)
    print("First run: wrote journal")
}

let balances = rebuildBalances(from: journalFileURL)
print("Rebuilt A: \(balances["A"] ?? 0)")
print("Rebuilt B: \(balances["B"] ?? 0)")
print("Journal at: \(journalFileURL.path)")

Task {
    for (id, balance) in balances {
        await bank.openAccount(id: id, balance: balance)
    }
    await bank.openAccount(id: "F", balance: 10)
    await bank.freeze(id: "F")

    do {
        try startServer(port: 9000)
    } catch {
        print("server failed to start: \(error)")
    }
}

dispatchMain()
