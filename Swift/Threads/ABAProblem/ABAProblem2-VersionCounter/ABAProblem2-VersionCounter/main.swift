//
//  main.swift
//  ABAProblem2-VersionCounter
//
//  Created by Anussha on 14/10/26.
//
import Foundation

final class Node {
    let value: Int
    var next: Node?
    init(_ value: Int, next: Node? = nil) {
        self.value = value
        self.next = next
    }
}

struct TaggedTop {
    var node: Node?
    var version: Int
}

final class VersionedStack {
    var top: TaggedTop

    init(top: TaggedTop) {
        self.top = top
    }

    // Succeeds only if BOTH the node and the version match
    func compareAndSwap(expected: TaggedTop, new: TaggedTop) -> Bool {
        if top.node === expected.node && top.version == expected.version {
            top = new
            return true
        }
        return false
    }
}

// Pop using a retry loop: read fresh state, try the CAS, retry if it fails
func pop(from stack: VersionedStack) -> Int? {
    while true {
        let current = stack.top
        guard let node = current.node else { return nil }  // empty stack
        let next = TaggedTop(node: node.next, version: current.version + 1)
        if stack.compareAndSwap(expected: current, new: next) {
            return node.value
        }
        // CAS failed: state changed, loop and read again
    }
}

let b = Node(2)
let a = Node(1, next: b)
let stack = VersionedStack(top: TaggedTop(node: a, version: 0))

let group = DispatchGroup()
var results = [Int?](repeating: nil, count: 2)
let lock = NSLock()

for i in 0..<2 {
    group.enter()
    DispatchQueue.global().async {
        let value = pop(from: stack)
        lock.lock()
        results[i] = value
        lock.unlock()
        group.leave()
    }
}

group.wait()
print("thread 0 popped:", results[0] ?? -1)
print("thread 1 popped:", results[1] ?? -1)
print("top node value:", stack.top.node?.value ?? -1)
print("top version:", stack.top.version)


/*
let b = Node(2)
let a = Node(1, next: b)
let stack = VersionedStack(top: TaggedTop(node: a, version: 0))

// Thread 2's work, done up front for this single-threaded test:
// pop A, pop B, push A back (version goes up each change)
stack.top = TaggedTop(node: b, version: 1)
stack.top = TaggedTop(node: nil, version: 2)
a.next = nil
stack.top = TaggedTop(node: a, version: 3)

// Thread 1 pops now, reading fresh state
let popped = pop(from: stack)
print("popped value:", popped ?? -1)
print("top node value:", stack.top.node?.value ?? -1)
print("top version:", stack.top.version)

*/


/*
let b = Node(2)
let a = Node(1, next: b)
let stack = VersionedStack(top: TaggedTop(node: a, version: 0))

// Thread 1 saves the top and its next
let saved = stack.top                       // (A, 0)
let savedNext = saved.node?.next            // B

// Thread 2 pops A, pops B, pushes A back (version goes up each change)
stack.top = TaggedTop(node: b, version: 1)  // pop A
stack.top = TaggedTop(node: nil, version: 2) // pop B
a.next = nil
stack.top = TaggedTop(node: a, version: 3)  // push A back

// Thread 1 resumes and tries its CAS with the old value
let succeeded = stack.compareAndSwap(
    expected: saved,
    new: TaggedTop(node: savedNext, version: saved.version + 1)
)
print("CAS succeeded:", succeeded)
print("top node value:", stack.top.node?.value ?? -1)
print("top version:", stack.top.version)
*/
/*
final class Node {
    let value: Int
    var next: Node?
    init(_ value: Int, next: Node? = nil) {
        self.value = value
        self.next = next
    }
}

// The pointer and the version travel together as one value
struct TaggedTop {
    var node: Node?
    var version: Int
}

let b = Node(2)
let a = Node(1, next: b)

let start = TaggedTop(node: a, version: 0)
print("node value:", start.node?.value ?? -1)
print("version:", start.version)

*/
