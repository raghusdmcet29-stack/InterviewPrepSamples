//
//  main.swift
//  Concurrency12-Barrier
//
//  Created by Anussha on 12/10/26.
//

import Foundation

final class Barrier {
    private let cond = NSCondition()
    private let total: Int
    private var arrived = 0

    init(total: Int) {
        self.total = total
    }

    func wait() {
        cond.lock()
        arrived += 1
        if arrived == total {
            cond.broadcast()
        } else {
            while arrived < total {
                cond.wait()
            }
        }
        cond.unlock()
    }
}

func worker(_ name: String, seconds: UInt32, barrier: Barrier) {
    sleep(seconds)
    print("\(name) arrived at barrier")
    barrier.wait()
    print("\(name) passed the barrier")
}

let barrier = Barrier(total: 3)
let group = DispatchGroup()
for (name, s) in [("A", 1), ("B", 2), ("C", 3)] {
    group.enter()
    DispatchQueue.global().async {
        worker(name, seconds: UInt32(s), barrier: barrier)
        group.leave()
    }
}
group.wait()
print("Done")



/*
import Foundation

func worker(_ name: String, seconds: UInt32) {
    sleep(seconds)
    print("\(name) arrived at barrier")
}

let group = DispatchGroup()
for (name, s) in [("A", 1), ("B", 2), ("C", 3)] {
    group.enter()
    DispatchQueue.global().async {
        worker(name, seconds: UInt32(s))
        group.leave()
    }
}
group.wait()
print("Done")
*/
/*
import Foundation

func worker(_ name: String, seconds: UInt32) {
    sleep(seconds)
    print("\(name) finished work")
}

let group = DispatchGroup()
for (name, s) in [("A", 1), ("B", 2), ("C", 3)] {
    group.enter()
    DispatchQueue.global().async {
        worker(name, seconds: UInt32(s))
        group.leave()
    }
}
group.wait()
print("Done")
*/
