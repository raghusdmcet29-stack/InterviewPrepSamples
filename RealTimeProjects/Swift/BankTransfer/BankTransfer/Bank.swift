//
//  Bank.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

import Foundation

final class Bank {
    private var accounts: [Int: Account] = [:]
    let successCount = UnsafeMutablePointer<Int64>.allocate(capacity: 1)
    let failedCount = UnsafeMutablePointer<Int64>.allocate(capacity: 1)

    init() {
        successCount.initialize(to: 0)
        failedCount.initialize(to: 0)
    }

    func open(id: Int, balance: Int) {
        accounts[id] = Account(id: id, balance: balance)
    }

    func balanceOf(_ id: Int) -> Int {
        accounts[id]!.balance
    }

    func transfer(_ r: TransferRequest) -> Bool {
        guard let from = accounts[r.from], let to = accounts[r.to] else {
            OSAtomicIncrement64(failedCount)
            return false
        }

        // Stage 3: always lock the lower-numbered account first
        let first = from.id < to.id ? from : to
        let second = from.id < to.id ? to : from

        first.lock.lock()
        second.lock.lock()

        // Check the balance while holding both locks
        if from.balance < r.amount {
            second.lock.unlock()
            first.lock.unlock()
            OSAtomicIncrement64(failedCount)
            return false
        }

        from.balance -= r.amount
        to.balance += r.amount

        second.lock.unlock()
        first.lock.unlock()

        // Stage 4: atomic success counter
        OSAtomicIncrement64(successCount)
        return true
    }
}
