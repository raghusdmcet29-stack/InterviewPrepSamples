//
//  main.swift
//  LRUCacheActor
//
//  Created by Anussha on 01/09/26.
//

import Foundation

final class Node<Key : Hashable,Value>{
    let key : Key
    var value : Value
    var prev : Node?
    var Next : Node?
    
    init(key: Key, value: Value) {
        self.key = key
        self.value = value
    }
}

actor LRUCache<Key : Hashable,Value>{
    private let capacity : Int
    private var dict : [Key : Node<Key,Value>] = [:]
    private var head : Node<Key, Value>? // most recently used
    private var tail : Node<Key,Value>? // least recently used
    
    init(capacity : Int){
        self.capacity = max(1, capacity)
    }
    
    private func remove(_ node : Node<Key,Value>){
        let prev = node.prev
        let next = node.Next
        
        prev?.Next = next
        next?.prev = prev
        
        if node === head { head = next }
        if node === tail { tail = prev}
        
        node.prev = nil
        node.Next = nil
    }
    
    private func insertAtHead(_ node : Node<Key,Value>){
        node.Next = head
        node.prev = nil
        head?.prev = node
        head = node
        
        if tail == nil { tail = node }
    }
    
    func get(_ key:Key) -> Value?{
        guard let node = dict[key] else { return nil }
        remove(node)
        insertAtHead(node)
        return node.value
    }
    
    func put(_ key : Key,_ value : Value){
        if var existing = dict[key]{
            existing.value = value
            remove(existing)
            insertAtHead(existing)
            return
        }
        
        let node = Node(key: key, value: value)
        dict[key] = node
        insertAtHead(node)
        
        if dict.count > capacity{
            if let lru = tail {
                remove(lru)
                dict.removeValue(forKey: lru.key)
            }
        }
    }
}

func runTest() async {
    let cache = LRUCache<String,Int>(capacity: 2)
    // Sequential trace matching our walkthrough
    await cache.put("A", 1)
    await cache.put("B", 2)
    let a = await cache.get("A")
    await cache.put("C", 3)
    
    print("get(A) mid-trace: \(a ?? -1)")                    // expect 1
    print("get(B) after eviction: \(await cache.get("B") ?? -1)")  // expect -1 (evicted)
    print("get(A) still present: \(await cache.get("A") ?? -1)")   // expect 1
    print("get(C) still present: \(await cache.get("C") ?? -1)")   // expect 3
    
    // Concurrent stress test: many tasks hitting put/get at once
        let bigCache = LRUCache<Int, Int>(capacity: 50)
        await withTaskGroup(of: Void.self) { group in
            for i in 0..<1000 {
                group.addTask {
                    await bigCache.put(i % 100, i)
                    _ = await bigCache.get(i % 100)
                }
            }
        }
        print("Concurrent stress test completed without crashing")
}

Task {
    await runTest()
    exit(0)
}

RunLoop.main.run()
