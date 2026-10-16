//
//  main.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//
import Foundation

let bank = Bank()
bank.open(id: 1, balance: 1000)
bank.open(id: 2, balance: 1000)

// Stage 1: threads
let t1 = Thread {
    for _ in 0..<1000 {
        _ = bank.transfer(TransferRequest(from: 1, to: 2, amount: 1))
    }
}
let t2 = Thread {
    for _ in 0..<1000 {
        _ = bank.transfer(TransferRequest(from: 2, to: 1, amount: 1))
    }
}

t1.start()
t2.start()
sleep(2)

print("== Stage 1-4 results ==")
print("A: \(bank.balanceOf(1)), B: \(bank.balanceOf(2)), total: \(bank.balanceOf(1) + bank.balanceOf(2))")
print("Successful transfers: \(bank.successCount.pointee)")
print("Failed transfers: \(bank.failedCount.pointee)")

// Stage 5-6: worker pool that starts together
print("== Stage 5-7 ==")

let queue = BoundedQueue(capacity: 3)
let workerCount = 2
let startLine = Barrier(parties: workerCount)
let bucket = TokenBucket(capacity: 2, refillPerSecond: 10)
let group = DispatchGroup()

for w in 1...workerCount {
    group.enter()
    Thread {
        AuditID.set("worker-\(w)")
        print("Worker \(w) arrived")
        startLine.wait()
        print("Worker \(w) released")
        while let request = queue.dequeue() {
            bucket.take()
           _ = withRetry(maxAttempts: 3, baseDelay: 0.1) {
                bank.transfer(request)
            }
            print("[\(AuditID.current())] moved \(request.amount) from \(request.from) to \(request.to)")
        }
        print("Worker \(w) stopped")
        group.leave()
    }.start()
}

let startTime = Date()

for _ in 1...10 {
    queue.enqueue(TransferRequest(from: 1, to: 2, amount: 10))
}
for _ in 1...workerCount {
    queue.enqueue(nil)
}

group.wait()

let elapsed = Date().timeIntervalSince(startTime)

print("Batch took \(String(format: "%.1f", elapsed)) seconds")
print("A: \(bank.balanceOf(1)), B: \(bank.balanceOf(2)), total: \(bank.balanceOf(1) + bank.balanceOf(2))")
print("Successful transfers: \(bank.successCount.pointee)")
print("Failed transfers: \(bank.failedCount.pointee)")

// Stage 8 test: fails twice, then succeeds
print("== Stage 8 test ==")
var tries = 0
let ok = withRetry(maxAttempts: 3, baseDelay: 0.1) {
    tries += 1
    print("Attempt \(tries)")
    return tries == 3
}
print("Succeeded: \(ok) after \(tries) attempts")

// Stage 10: task group summary
print("== Stage 10 summary ==")
let summaryDone = DispatchSemaphore(value: 0)

Task {
    let summary = await summarize(bank: bank, accountIDs: [1, 2])
    for id in [1, 2] {
        print("Account \(id): \(summary.balances[id]!)")
    }
    print("Total: \(summary.total)")
    print("Failed transfers: \(bank.failedCount.pointee)")
    summaryDone.signal()
}

summaryDone.wait()
