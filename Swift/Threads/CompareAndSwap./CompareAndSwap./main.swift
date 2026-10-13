//
//  main.swift
//  CompareAndSwap.
//
//  Created by Anussha on 13/10/26.
//


import Foundation

let value = UnsafeMutablePointer<Int64>.allocate(capacity: 1)
value.pointee = 0

func addOne() {
    var done = false
    while !done {
        let old = value.pointee
        done = OSAtomicCompareAndSwap64(old, old + 1, value)
    }
}

let group = DispatchGroup()
for _ in 0..<2 {
    DispatchQueue.global().async(group: group) {
        for _ in 0..<1000 {
            addOne()
        }
    }
}
group.wait()
print("Final value: \(value.pointee)")
value.deallocate()


/*
import Foundation

let value = UnsafeMutablePointer<Int64>.allocate(capacity: 1)
value.pointee = 5

let ok1 = OSAtomicCompareAndSwap64(5, 6, value)
print("First CAS (expect 5, set 6): \(ok1), value now \(value.pointee)")

let ok2 = OSAtomicCompareAndSwap64(5, 7, value)
print("Second CAS (expect 5, set 7): \(ok2), value now \(value.pointee)")

value.deallocate()
*/
