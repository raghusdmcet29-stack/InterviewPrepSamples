//
//  main.swift
//  TicketBooking
//
//  Created by Anussha on 15/10/26.
//

import Foundation

let lock = NSLock()
var seats = [Bool](repeating: false, count: 3)

// Barrier: each thread waits until `total` threads have arrived.
let gate = NSCondition()
var arrived = 0
let total = 3


func waitAtGate(){
    gate.lock()
    arrived += 1
    if arrived == total{
        gate.broadcast() // last one in opens the gate
    }else {
        while arrived < total {
            gate.wait()
        }
    }
    gate.unlock()
}

// Compare-and-swap counter: adds 1 only if the value is still what we read.
final class AtomicCounter {
    private var value: Int = 0
    private let guardLock = NSLock()   // stands in for the hardware CAS instruction

    func compareAndSwap(expected: Int, new: Int) -> Bool {
        guardLock.lock()
        defer { guardLock.unlock() }
        if value == expected {
            value = new
            return true
        }
        return false
    }

    func read() -> Int {
        guardLock.lock()
        defer { guardLock.unlock() }
        return value
    }
}

let bookings = AtomicCounter()

func increment(_ counter: AtomicCounter) {
    while true {
        let old = counter.read()
        if counter.compareAndSwap(expected: old, new: old + 1) { return }
    }
}

// Thread-local storage: each thread keeps its own receipt number.
let receiptKey = "receipt"

struct BookingResult {
    let customer: Int
    let receipt: Int
    let booked: Bool
}

func bookSeat(customer: Int) -> BookingResult {
    Thread.current.threadDictionary[receiptKey] = 100 + customer
    waitAtGate()
    
    var booked = false
    lock.lock()
    if !seats[customer - 1] {
        seats[customer - 1] = true
        booked = true
      increment(bookings)
    }
    lock.unlock()
    let receipt = Thread.current.threadDictionary[receiptKey] as! Int
    return BookingResult(customer: customer, receipt: receipt, booked: booked)
}

// Task group: run all customers in parallel, then collect every result.
let results = await withTaskGroup(of: BookingResult.self) { group in
    
    for customer in 1...3 {
        group.addTask { bookSeat(customer: customer) }
    }
    var collected: [BookingResult] = []
    for await result in group {
        collected.append(result)
    }
    return collected.sorted { $0.customer < $1.customer }
}

// Summary, printed once all results are in.
for r in results {
    print("customer \(r.customer): receipt \(r.receipt), booked: \(r.booked)")
}

print("seats: \(seats)")
print("total bookings: \(bookings.read())")
