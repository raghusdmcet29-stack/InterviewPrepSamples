//
//  main.swift
//  BoundedProducerConsumer
//
//  Created by Anussha on 07/10/26.
//

import Foundation

final class BoundedQueue {
    private var items: [Int] = []
    private let capacity: Int
    private let cond = NSCondition()
    
    init(capacity: Int) {
        self.capacity = capacity
    }
    
    func add(_ item: Int) {
        cond.lock()
        while items.count == capacity{
            print("Queue full, producer waiting")
            cond.wait()
        }
        items.append(item)
        print("Added \(item), queue size \(items.count)")
        cond.signal()
        cond.unlock()
    }
    
    func take() -> Int {
        cond.lock()
        while items.isEmpty {
            cond.wait()
        }
        let item = items.removeFirst()
        print("Took \(item), queue size \(items.count)")
        cond.signal()
        cond.unlock()
        return item
    }
}
let queue = BoundedQueue(capacity: 2)
let group = DispatchGroup()

group.enter()
DispatchQueue.global().async {
    for _ in 1...5 {
        sleep(1)
        _ = queue.take()
    }
    group.leave()
}

for i in 1...5 {
    queue.add(i)
}
print("Producer finished")
group.wait()
print("Done")
