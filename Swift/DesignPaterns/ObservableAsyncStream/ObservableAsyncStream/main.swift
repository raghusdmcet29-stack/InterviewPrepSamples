//
//  main.swift
//  ObservableAsyncStream
//
//  Created by Anussha on 31/08/26.
//

import Foundation
/*// Using single clouser implementation
final class ObservableValue<T>{
    private var onChange : ((T) -> Void)?
    
    var value : T {
        didSet {
            onChange?(value)
        }
    }
    
    init(_ initial : T) {
        self.value = initial
    }
    
    func observe(_ handler : @escaping (T) -> Void){
        self.onChange = handler
    }
}

let counter = ObservableValue<Int>(0)
counter.observe { newValue in
    print("Changed to \(newValue)")
}

counter.value = 1
counter.value = 2
counter.value = 3
*/

// multiple concurrent subscribers

final class ObservableValue<T> {
    private var continuations : [UUID : AsyncStream<T>.Continuation] = [:]
    private let lock = NSLock()
    
    var subscriberCount: Int {
            lock.lock()
            defer { lock.unlock() }
            return continuations.count
        }
    
    var value : T {
        didSet {
            lock.lock()
            let all = continuations.values
            lock.unlock()
            for continuation in all {
                continuation.yield(value)
            }
        }
    }
    
    init(_ initial: T) {
        self.value = initial
    }
    
    func stream() -> AsyncStream<T> {
        let id = UUID()
        return AsyncStream { continuation in
            lock.lock()
            continuations[id] = continuation
            lock.unlock()
            
            continuation.onTermination = { [weak self] _ in
                self?.lock.lock()
                self?.continuations.removeValue(forKey: id)
                self?.lock.unlock()
            }
        }
    }
}

let counter = ObservableValue<Int>(0)

let task = Task {
    for await v in counter.stream() {
        print("Got: \(v)")
    }
    print("Stream ended")
}

try await Task.sleep(nanoseconds: 200_000_000)
print("Subscriber count after subscribe: \(counter.subscriberCount)")

task.cancel()

try await Task.sleep(nanoseconds: 200_000_000)
print("Subscriber count after cancel: \(counter.subscriberCount)")

/*let counter = ObservableValue<Int>(0)

let task1 = Task {
    for await v in counter.stream().prefix(3) {
        print("Subscriber 1 got: \(v)")
    }
}

let task2 = Task {
    for await v in counter.stream().prefix(3) {
        print("Subscriber 2 got: \(v)")
    }
}

// give subscribers a moment to attach before mutating
try await Task.sleep(nanoseconds: 200_000_000)

counter.value = 1
counter.value = 2
counter.value = 3

await task1.value
await task2.value
*/

/*let counter = ObservableValue<Int>(0)

// Mutate before anyone subscribes
counter.value = 1
counter.value = 2

let lateTask = Task {
    for await v in counter.stream().prefix(1) {
        print("Late subscriber got: \(v)")
    }
}

// give the late subscriber a moment to attach
try await Task.sleep(nanoseconds: 200_000_000)

counter.value = 3

await lateTask.value */
