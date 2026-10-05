//
//  main.swift
//  ProducerConsumer
//
//  Created by Anussha on 05/10/26.
//

import Foundation

let cond = NSCondition()
var queue: [Int] = []
let group = DispatchGroup()

// Consumer: takes 3 items
group.enter()
DispatchQueue.global().async {
    for _ in 1...3 {
        cond.lock()
        while queue.isEmpty {
            cond.wait()
        }
        let item = queue.removeFirst()
        print("Consumer took \(item)")
        cond.unlock()
    }
    group.leave()
}

// Producer: adds items 1, 2, 3, one per second
group.enter()
DispatchQueue.global().async {
    for item in 1...3 {
        Thread.sleep(forTimeInterval: 1)
        cond.lock()
        queue.append(item)
        print("Producer added \(item)")
        cond.signal()
        cond.unlock()
    }
    group.leave()
}

group.wait()
print("Done")



