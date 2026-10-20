//
//  Journal.swift
//  ActorBank
//
//  Created by Anussha on 20/10/26.
//

import Foundation

let journalFileURL = FileManager.default.homeDirectoryForCurrentUser
    .appendingPathComponent("ActorBankData")
    .appendingPathComponent("journal.txt")

func rebuildBalances(from fileURL: URL) -> [String: Int] {
    let readBack = (try? String(contentsOf: fileURL, encoding: .utf8)) ?? ""
    var balances: [String: Int] = [:]
    for line in readBack.split(separator: "\n") {
        if let entry = JournalEntry.parse(String(line)) {
            balances[entry.accountId, default: 0] += entry.amount
        }
    }
    return balances
}

func nextSequenceNumber(from fileURL: URL) -> Int {
    let readBack = (try? String(contentsOf: fileURL, encoding: .utf8)) ?? ""
    guard let last = readBack.split(separator: "\n").last,
          let entry = JournalEntry.parse(String(last)) else {
        return 1
    }
    return entry.sequence + 1
}

func appendToJournal(_ entry: JournalEntry, to fileURL: URL) -> Bool {
    guard let data = (entry.line + "\n").data(using: .utf8),
          let handle = try? FileHandle(forWritingTo: fileURL) else {
        return false
    }
    defer { try? handle.close() }
    do {
        try handle.seekToEnd()
        try handle.write(contentsOf: data)
        return true
    } catch {
        return false
    }
}

func appendTextToJournal(_ text: String, to fileURL: URL) -> Bool {
    guard let data = text.data(using: .utf8),
          let handle = try? FileHandle(forWritingTo: fileURL) else {
        return false
    }
    defer { try? handle.close() }
    do {
        try handle.seekToEnd()
        try handle.write(contentsOf: data)
        return true
    } catch {
        return false
    }
}

actor JournalWriter {
    let fileURL: URL
    private var nextSequence: Int

    init(fileURL: URL) {
        self.fileURL = fileURL
        self.nextSequence = nextSequenceNumber(from: fileURL)
    }

    func record(accountId: String, amount: Int) -> Bool {
        let entry = JournalEntry(sequence: nextSequence, accountId: accountId, amount: amount)
        guard appendToJournal(entry, to: fileURL) else {
            return false
        }
        nextSequence += 1
        return true
    }
    
    func recordTransfer(from: String, to: String, amount: Int) -> Bool {
        let out = JournalEntry(sequence: nextSequence, accountId: from, amount: -amount)
        let into = JournalEntry(sequence: nextSequence + 1, accountId: to, amount: amount)
        let text = out.line + "\n" + into.line + "\n"
        guard appendTextToJournal(text, to: fileURL) else {
            return false
        }
        nextSequence += 2
        return true
    }
}
