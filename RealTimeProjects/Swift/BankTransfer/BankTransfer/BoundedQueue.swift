//
//  BoundedQueue.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

import Foundation

final class BoundedQueue {
    private var items: [TransferRequest?] = []
    private let capacity: Int
    private let condition = NSCondition()

    init(capacity: Int) {
        self.capacity = capacity
    }

    func enqueue(_ item: TransferRequest?) {
        condition.lock()
        while items.count == capacity {
            condition.wait()
        }
        items.append(item)
        condition.broadcast()
        condition.unlock()
    }

    func dequeue() -> TransferRequest? {
        condition.lock()
        while items.isEmpty {
            condition.wait()
        }
        let item = items.removeFirst()
        condition.broadcast()
        condition.unlock()
        return item
    }
}
