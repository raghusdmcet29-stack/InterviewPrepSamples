//
//  main.swift
//  ActorBank
//
//  Created by Anussha on 19/10/26.
//

import Foundation

let folder = FileManager.default.homeDirectoryForCurrentUser
    .appendingPathComponent("ActorBankData")
try? FileManager.default.createDirectory(at: folder, withIntermediateDirectories: true)
let fileURL = folder.appendingPathComponent("journal.txt")

if !FileManager.default.fileExists(atPath: fileURL.path) {
    let entries = [
        JournalEntry(sequence: 1, accountId: "A", amount: 100),
        JournalEntry(sequence: 2, accountId: "A", amount: -30),
        JournalEntry(sequence: 3, accountId: "B", amount: 50)
    ]
    let text = entries.map { $0.line }.joined(separator: "\n") + "\n"
    _ = try? text.write(to: fileURL, atomically: true, encoding: .utf8)
    print("First run: wrote journal")
}

let readBack = (try? String(contentsOf: fileURL, encoding: .utf8)) ?? ""
var balances: [String: Int] = [:]
for line in readBack.split(separator: "\n") {
    if let entry = JournalEntry.parse(String(line)) {
        balances[entry.accountId, default: 0] += entry.amount
    }
}

print("Rebuilt A: \(balances["A"] ?? 0)")
print("Rebuilt B: \(balances["B"] ?? 0)")
print("Journal at: \(fileURL.path)")
