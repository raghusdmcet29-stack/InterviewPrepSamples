//
//  main.swift
//  ABAProblem1-StaleNext
//
//  Created by Anussha on 14/10/26.
//

final class Node {
    let value: Int
    var next: Node?
    init(_ value: Int, next: Node? = nil) {
        self.value = value
        self.next = next
    }
}

final class Stack {
    var top: Node?

    // Simulated CAS: swap `top` only if it is still the same node object.
    func compareAndSwapTop(expected: Node?, new: Node?) -> Bool {
        if top === expected {
            top = new
            return true
        }
        return false
    }
}

let b = Node(2)
let a = Node(1, next: b)
let stack = Stack()
stack.top = a

// Thread 1 reads the top and its next, then pauses
let expectedTop = stack.top        // A
let savedNext = expectedTop?.next  // B

// Thread 2 pops A, pops B, pushes A back
stack.top = a.next                 // top = B
stack.top = b.next                 // top = nil
a.next = nil
stack.top = a                      // top = A again

// Thread 1 resumes and tries its CAS
let succeeded = stack.compareAndSwapTop(expected: expectedTop, new: savedNext)
print("CAS succeeded:", succeeded)
print("top value:", stack.top?.value ?? -1)

