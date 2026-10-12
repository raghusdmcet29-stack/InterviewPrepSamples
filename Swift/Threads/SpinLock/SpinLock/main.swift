//
//  main.swift
//  SpinLock
//
//  Created by Anussha on 12/10/26.
//

import Foundation

let flag = UnsafeMutablePointer<Int64>.allocate(capacity: 1)
flag.initialize(to: 0)
var counter = 0

func spinLock() {
    while !OSAtomicCompareAndSwap64(0, 1, flag) {
        // spin until the flag was 0 and we set it to 1
    }
}

func spinUnlock() {
    _ = OSAtomicCompareAndSwap64(1, 0, flag)
}

func work(_ name: String) {
    spinLock()
    counter += 1
    print("\(name) added, counter = \(counter)")
    spinUnlock()
}

let group = DispatchGroup()
for name in ["T1", "T2", "T3"] {
    group.enter()
    DispatchQueue.global().async {
        work(name)
        group.leave()
    }
}
group.wait()
print("Done, final counter = \(counter)")


// using Nslock as spin
/*import Foundation

let lock = NSLock()
var counter = 0

func work(_ name: String) {
    while !lock.try() {
        // spin: keep trying until we get the lock
    }
    counter += 1
    print("\(name) added, counter = \(counter)")
    lock.unlock()
}

let group = DispatchGroup()
for name in ["T1", "T2", "T3"] {
    group.enter()
    DispatchQueue.global().async {
        work(name)
        group.leave()
    }
}
group.wait()
print("Done, final counter = \(counter)")
*/
