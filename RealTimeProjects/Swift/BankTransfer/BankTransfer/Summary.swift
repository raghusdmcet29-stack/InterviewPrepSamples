//
//  Summary.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

import Foundation

func summarize(bank: Bank, accountIDs: [Int]) async -> (balances: [Int: Int], total: Int) {
    await withTaskGroup(of: (Int, Int).self) { group in
        for id in accountIDs {
            group.addTask {
                (id, bank.balanceOf(id))
            }
        }
        var balances: [Int: Int] = [:]
        for await (id, balance) in group {
            balances[id] = balance
        }
        let total = balances.values.reduce(0, +)
        return (balances, total)
    }
}
