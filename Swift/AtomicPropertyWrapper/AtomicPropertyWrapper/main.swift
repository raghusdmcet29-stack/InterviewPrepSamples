//
//  main.swift
//  AtomicPropertyWrapper
//
//  Created by Anussha on 31/08/26.
//

import Foundation
/*  which will create race conditon miss few incrments
final class UnsafeCounter {
    var value : Int = 0
}

let unsafeCounter = UnsafeCounter()

DispatchQueue.concurrentPerform(iterations: 1000){ _ in
    unsafeCounter.value += 1
}

print("Unsafe counter final value: \(unsafeCounter.value)")
*/

@propertyWrapper
struct Atomic<Value> {
    private var value : Value
    private let lock = NSLock()
    
    init(wrappedValue: Value) {
        self.value = wrappedValue
    }
    
    var wrappedValue : Value {
        get {
            lock.lock()
            defer {lock .unlock()}
            return value
        }
        set {
            lock.lock()
            defer {lock.unlock()}
            value = newValue
        }
    }
    
    // The real fix: mutate under a single lock acquisition
       mutating func mutate(_ transform: (inout Value) -> Void) {
           lock.lock()
           defer { lock.unlock() }
           transform(&value)
       }
}

final class SafeCounter {
    @Atomic private(set) var value : Int = 0
    
    func increment() {
            _value.mutate { $0 += 1 }
        }
}

let safeCounter = SafeCounter()

DispatchQueue.concurrentPerform(iterations: 1000){ _ in
    safeCounter.increment()
}

print("Safe counter final value: \(safeCounter.value)")
