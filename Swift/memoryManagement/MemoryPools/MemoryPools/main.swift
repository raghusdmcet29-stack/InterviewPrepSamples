//
//  main.swift
//  MemoryPools
//
//  Created by Anussha on 30/09/26.
//
import Foundation

final class IntPool {
    let capacity: Int
    let block: UnsafeMutablePointer<Int>
    var nextFree: Int = 0
    var returned: [Int] = []

    init(capacity: Int) {
        self.capacity = capacity
        self.block = UnsafeMutablePointer<Int>.allocate(capacity: capacity)
        print("Pool created: \(capacity) slots")
    }

    func allocateSlot() -> UnsafeMutablePointer<Int>? {
        if let index = returned.popLast() {
            return block + index
        }
        if nextFree >= capacity {
            return nil
        }
        let slot = block + nextFree
        nextFree += 1
        return slot
    }

    func giveBack(_ slot: UnsafeMutablePointer<Int>) {
        let index = slot - block
        returned.append(index)
    }

    deinit {
        block.deallocate()
        print("Pool freed")
    }
}

do {
    let pool = IntPool(capacity: 4)

    var slots: [UnsafeMutablePointer<Int>] = []
    for i in 1...4 {
        if let slot = pool.allocateSlot() {
            slot.pointee = i * 10
            slots.append(slot)
        }
    }

    pool.giveBack(slots[0])
    pool.giveBack(slots[3])

    for i in 1...3 {
        if let slot = pool.allocateSlot() {
            print("Request \(i): got slot \(slot - pool.block)")
        } else {
            print("Request \(i): refused")
        }
    }
}
