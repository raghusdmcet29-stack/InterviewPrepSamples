//
//  main.swift
//  GenericCacheDemo
//
//  Created by Anussha on 25/08/26.
//

import Foundation

protocol EvictionPolicy {
    associatedtype Key
    mutating func recordAccess(_ key: Key)
    mutating func keyToEvict() -> Key?
}

struct FIFOEvictionPolicy<Key: Hashable>: EvictionPolicy {
    private var order: [Key] = []

    mutating func recordAccess(_ key: Key) {
        // only track new keys, avoid duplicate entries
        if !order.contains(key) {
            order.append(key)
        }
    }

    mutating func keyToEvict() -> Key? {
        order.isEmpty ? nil : order.removeFirst()
    }
}
// with eviction policy
class Cache<Key: Hashable, Value, Policy: EvictionPolicy> where Policy.Key == Key {
    private var storage: [Key: Value] = [:]
    private var policy: Policy
    private let maxSize: Int
    
    init(maxSize: Int, policy: Policy) {
            self.maxSize = maxSize
            self.policy = policy
        }
    
    func set(_ value: Value, forKey key: Key) {
            if storage[key] == nil && storage.count >= maxSize {
                if let evictKey = policy.keyToEvict() {
                    storage.removeValue(forKey: evictKey)
                    print("Evicted key: \(evictKey)")
                }
            }
            storage[key] = value
            policy.recordAccess(key)
        }
    func get(_ key: Key) -> Value? {
            storage[key]
    }

    var count: Int {
            storage.count
    }
}

let policy = FIFOEvictionPolicy<String>()
let cache = Cache<String, Int, FIFOEvictionPolicy<String>>(maxSize: 3, policy: policy)
cache.set(1, forKey: "a")
cache.set(2, forKey: "b")
cache.set(3, forKey: "c")
print("Count after 3 inserts: \(cache.count)")
cache.set(4, forKey: "d")   // cache is full, should evict "a" (oldest)
print("Count after 4th insert: \(cache.count)")
print("a: \(cache.get("a") as Any)")   // should be nil, evicted
print("d: \(cache.get("d") as Any)")   // should be 4
// below code without evaction policy
/*class Cache<Key: Hashable, Value> {
    private var storage: [Key: Value] = [:]

    func set(_ value: Value, forKey key: Key) {
            storage[key] = value
    }

    func get(_ key: Key) -> Value? {
            storage[key]
    }

    func remove(_ key: Key) {
            storage.removeValue(forKey: key)
    }

    var count: Int {
            storage.count
    }
}

let cache = Cache<String, Int>()

cache.set(100, forKey: "apple")
cache.set(200, forKey: "banana")
print(cache.get("apple") as Any)
print(cache.get("banana") as Any)
print(cache.get("cherry") as Any)
print("Count: \(cache.count)")

cache.remove("apple")
print("After remove, count: \(cache.count)")
print(cache.get("apple") as Any)*/

