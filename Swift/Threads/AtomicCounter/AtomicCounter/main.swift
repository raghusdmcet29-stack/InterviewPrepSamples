//
//  main.swift
//  AtomicCounter
//
//  Created by Anussha on 07/10/26.
//
// using atomic and mutex both

import Foundation

let threadCount = 4
let incrementsPerThread = 1_000_000

final class MutexCounter {
    private let lock = NSLock()
    private var value = 0

    func increment() {
        lock.lock()
        value += 1
        lock.unlock()
    }

    func read() -> Int {
        lock.lock()
        defer { lock.unlock() }
        return value
    }
}

final class AtomicCounter {
    private let value: UnsafeMutablePointer<Int64>

    init() {
        value = UnsafeMutablePointer<Int64>.allocate(capacity: 1)
        value.initialize(to: 0)
    }

    deinit {
        value.deallocate()
    }

    func increment() {
        OSAtomicIncrement64(value)
    }

    func read() -> Int64 {
        return value.pointee
    }
}

let clock = ContinuousClock()

let mutexCounter = MutexCounter()
let mutexTime = clock.measure {
    DispatchQueue.concurrentPerform(iterations: threadCount) { _ in
        for _ in 0..<incrementsPerThread {
            mutexCounter.increment()
        }
    }
}
print("Mutex counter: \(mutexCounter.read()) (expected \(threadCount * incrementsPerThread))")
print("Mutex time: \(mutexTime)")

let atomicCounter = AtomicCounter()
let atomicTime = clock.measure {
    DispatchQueue.concurrentPerform(iterations: threadCount) { _ in
        for _ in 0..<incrementsPerThread {
            atomicCounter.increment()
        }
    }
}
print("Atomic counter: \(atomicCounter.read()) (expected \(threadCount * incrementsPerThread))")
print("Atomic time: \(atomicTime)")

// using mutex avoiding data race
/*import Foundation

let threadCount = 4
let incrementsPerThread = 1_000_000

final class MutexCounter {
    private let lock = NSLock()
    private var value = 0

    func increment() {
        lock.lock()
        value += 1
        lock.unlock()
    }

    func read() -> Int {
        lock.lock()
        defer { lock.unlock() }
        return value
    }
}

let mutexCounter = MutexCounter()

DispatchQueue.concurrentPerform(iterations: threadCount) { _ in
    for _ in 0..<incrementsPerThread {
        mutexCounter.increment()
    }
}

print("Mutex counter: \(mutexCounter.read()) (expected \(threadCount * incrementsPerThread))")
*/

//data race
/*
import Foundation

let threadCount = 4
let incrementsPerThread = 1_000_000

final class PlainCounter {
    var value = 0
}

let counter = PlainCounter()

DispatchQueue.concurrentPerform(iterations: threadCount) { _ in
    for _ in 0..<incrementsPerThread {
        counter.value += 1
    }
}

print("Plain counter: \(counter.value) (expected \(threadCount * incrementsPerThread))")

*/
