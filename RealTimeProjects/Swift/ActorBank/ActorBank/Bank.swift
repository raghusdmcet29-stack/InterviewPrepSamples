//
//  Bank.swift
//  ActorBank
//
//  Created by Anussha on 19/10/26.
//

import Foundation

actor Bank {
    private var accounts: [String: Account] = [:]
    private let journalURL: URL
    private var nextSequence: Int

    init(journalURL: URL) {
        self.journalURL = journalURL
        self.nextSequence = nextSequenceNumber(from: journalURL)
    }

    func openAccount(id: String, balance: Int) {
        accounts[id] = Account(id: id, balance: balance)
    }

    func freeze(id: String) {
        accounts[id]?.freeze()
    }

    func balance(of id: String) -> Int? {
        accounts[id]?.balance
    }

    func deposit(account id: String, amount: Int) -> Bool {
        guard let acct = accounts[id], !acct.isFrozen else {
            return false
        }
        let entry = JournalEntry(sequence: nextSequence, accountId: id, amount: amount)
        guard appendToJournal(entry, to: journalURL) else {
            return false
        }
        nextSequence += 1
        return acct.deposit(amount)
    }

    func transfer(from fromId: String, to toId: String, amount: Int) -> Bool {
        guard let from = accounts[fromId], let to = accounts[toId],
              !to.isFrozen, from.balance >= amount else {
            return false
        }
        let out = JournalEntry(sequence: nextSequence, accountId: fromId, amount: -amount)
        let into = JournalEntry(sequence: nextSequence + 1, accountId: toId, amount: amount)
        let text = out.line + "\n" + into.line + "\n"
        guard appendTextToJournal(text, to: journalURL) else {
            return false
        }
        nextSequence += 2
        _ = from.withdraw(amount)
        _ = to.deposit(amount)
        return true
    }
}
