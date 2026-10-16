//
//  main.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//


import Foundation

// MARK: - Stage 1: threads
print("== Stage 1: threads ==")
let s1a = Thread { print("Transfer A starting") }
let s1b = Thread { print("Transfer B starting") }
s1a.start()
s1b.start()
sleep(1)

// MARK: - Stage 2 and 3: per-account locks with lock ordering
print("== Stage 2 and 3: locks and lock ordering ==")

class Account {
    let id: Int
    var balance: Int
    let lock = NSLock()

    init(id: Int, balance: Int) {
        self.id = id
        self.balance = balance
    }
}

func transfer(from: Account, to: Account, amount: Int) {
    let first = from.id < to.id ? from : to
    let second = from.id < to.id ? to : from

    first.lock.lock()
    second.lock.lock()
    from.balance -= amount
    to.balance += amount
    second.lock.unlock()
    first.lock.unlock()
}

let a = Account(id: 1, balance: 1000)
let b = Account(id: 2, balance: 1000)

let s2t1 = Thread {
    for _ in 0..<1000 { transfer(from: a, to: b, amount: 1) }
}
let s2t2 = Thread {
    for _ in 0..<1000 { transfer(from: b, to: a, amount: 1) }
}
s2t1.start()
s2t2.start()
sleep(2)

print("A: \(a.balance), B: \(b.balance), total: \(a.balance + b.balance)")

// MARK: - Stage 4: atomic counters
print("== Stage 4: atomic counters ==")

let successCount = UnsafeMutablePointer<Int64>.allocate(capacity: 1)
successCount.initialize(to: 0)
let failedCount = UnsafeMutablePointer<Int64>.allocate(capacity: 1)
failedCount.initialize(to: 0)

let s4t1 = Thread {
    for _ in 0..<1000 { OSAtomicIncrement64(successCount) }
}
let s4t2 = Thread {
    for _ in 0..<1000 { OSAtomicIncrement64(failedCount) }
}
s4t1.start()
s4t2.start()
sleep(1)

print("Successful transfers: \(successCount.pointee)")
print("Failed transfers: \(failedCount.pointee)")
