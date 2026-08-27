//
//  main.swift
//  CustomSequenceDemo
//
//  Created by Anussha on 27/08/26.
//

import Foundation

struct FibonacciSequence: Sequence {
    let limit: Int

    func makeIterator() -> FibonacciIterator {
        return FibonacciIterator(limit: limit)
    }
}

struct FibonacciIterator: IteratorProtocol {
    let limit: Int
    var current = 0
    var upcoming = 1
    var count = 0

    mutating func next() -> Int? {
        guard count < limit else { return nil }
        let result = current
        let newUpcoming = current + upcoming
        current = upcoming
        upcoming = newUpcoming
        count += 1
        return result
    }
}

let fib = FibonacciSequence(limit: 10)

print("For-loop iteration:")
for num in fib {
    print(num, terminator: " ")
}
print()

print("Using map:")
let doubled = fib.map { $0 * 2 }
print(doubled)

print("Using filter:")
let evens = fib.filter { $0 % 2 == 0 }
print(evens)

print("Using first(where:):")
if let firstOver20 = fib.first(where: { $0 > 20 }) {
    print("First Fibonacci number over 20:", firstOver20)
}
