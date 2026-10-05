//
//  main.swift
//  Deadlock
//
//  Created by Anussha on 05/10/26.
//

// Avoiding deadlock by using lock in order

import Foundation

let lock1 = NSLock()
let lock2 = NSLock()
let group = DispatchGroup()

// Thread A: Lock 1 first, then Lock 2
group.enter()
DispatchQueue.global().async {
    lock1.lock()
    print("A took Lock 1")
    Thread.sleep(forTimeInterval: 1)
    print("A wants Lock 2")
    lock2.lock()
    print("A took Lock 2")
    lock2.unlock()
    lock1.unlock()
    group.leave()
}

// Thread B: now also Lock 1 first, then Lock 2
group.enter()
DispatchQueue.global().async {
    lock1.lock()                                  // CHANGED: was lock2
    print("B took Lock 1")                        // CHANGED
    Thread.sleep(forTimeInterval: 1)
    print("B wants Lock 2")                       // CHANGED
    lock2.lock()                                  // CHANGED: was lock1
    print("B took Lock 2")                        // CHANGED
    lock2.unlock()                                // CHANGED
    lock1.unlock()                                // CHANGED
    group.leave()
}

group.wait()
print("Done")

// below code is creating dead lock
/*import Foundation

let lock1 = NSLock()
let lock2 = NSLock()
let group = DispatchGroup()

// Thread A: Lock 1 first, then Lock 2
group.enter()
DispatchQueue.global().async {
    lock1.lock()
    print("A took Lock 1")
    Thread.sleep(forTimeInterval: 1)
    print("A wants Lock 2")
    lock2.lock()
    print("A took Lock 2")
    lock2.unlock()
    lock1.unlock()
    group.leave()
}

// Thread B: Lock 2 first, then Lock 1 (the opposite order)
group.enter()
DispatchQueue.global().async {
    lock2.lock()
    print("B took Lock 2")
    Thread.sleep(forTimeInterval: 1)
    print("B wants Lock 1")
    lock1.lock()
    print("B took Lock 1")
    lock1.unlock()
    lock2.unlock()
    group.leave()
}

group.wait()
print("Done")
*/
