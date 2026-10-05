//
//  main.swift
//  DataRace
//
//  Created by Anussha on 05/10/26.
//

import Foundation

// Avoiding data race using actor
actor Counter {
    private var value = 0
    
    func increment(){
        value += 1
    }
    func getvalue() -> Int{
        return value
    }
}

let counter = Counter()

await withTaskGroup(of: Void .self) { group in
    for _ in 0..<100000 {
        group.addTask {
            await counter.increment()
        }
    }
}

print("Expected: 100000")
print("Actual: \(await counter.getvalue())")
// Avoiding data race using lock
/*
var counter = 0
let lock = NSLock()

DispatchQueue.concurrentPerform(iterations: 100000) { _ in
    lock.lock()
    counter += 1
    lock.unlock()
}

print("Expected: 100000")
print("Actual: \(counter)")
*/

// Data race below function
/*var counter = 0

DispatchQueue.concurrentPerform(iterations: 100000) { _ in
    counter += 1
}

print("Expected: 100000")
print("Actual: \(counter)")
*/
