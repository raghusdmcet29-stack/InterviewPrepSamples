//
//  JournalEntry.swift
//  ActorBank
//
//  Created by Anussha on 19/10/26.
//

import Foundation

// JournalEntry.swift
struct JournalEntry {
    let sequence: Int
    let accountId: String
    let amount: Int
    var line: String {
        "\(sequence),\(accountId),\(amount)"
    }
    
    static func parse(_ text: String) -> JournalEntry?{
        let parts = text.split(separator: ",").map(String.init)
        guard parts.count == 3,
              let sequence = Int(parts[0]),
              let amount = Int(parts[2]) else {
            return nil
        }
        return JournalEntry(sequence: sequence, accountId: parts[1], amount: amount)
    }
}


