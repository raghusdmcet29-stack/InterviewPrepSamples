//
//  main.swift
//  Actors
//
//  Created by Anussha on 08/10/26.
//
// Actor to remove data race in counter
import Foundation

// Actor version: exact total. A plain class here loses increments (data race).
actor Counter {
    var value = 0

    func increment() {
        value += 1
    }
}

let counter = Counter()

await withTaskGroup(of: Void.self) { group in
    for _ in 1...4 {
        group.addTask {
            for _ in 1...1000 {
                await counter.increment()
            }
        }
    }
}

print(await counter.value)

// with out actor count incrment datarace
/*import Foundation

final class Counter {
    var value = 0

    func increment() {
        value += 1
    }
}

let counter = Counter()

await withTaskGroup(of: Void.self) { group in
    for _ in 1...4 {
        group.addTask {
            for _ in 1...1000 {
                counter.increment()
            }
        }
    }
}

print(counter.value)
 */


/*
import Foundation

actor Counter {
    var value = 0

    func increment() {
        value += 1
    }
}

let counter = Counter()
await counter.increment()
await counter.increment()
print(await counter.value)
*/
