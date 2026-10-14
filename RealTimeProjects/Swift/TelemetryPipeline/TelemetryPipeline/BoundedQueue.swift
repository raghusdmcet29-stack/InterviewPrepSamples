//
//  BoundedQueue.swift
//  TelemetryPipeline
//
//  Created by Anussha on 14/10/26.
//

import Foundation

final class BoundedQueue<T>{
    private let capacity : Int
    private var items :[T] = []
    private let condition = NSCondition()
    private var closed = false
    
    init(capacity : Int){
        self.capacity = capacity
    }
    
    func enqueue(_ item : T){
        condition.lock()
        defer {condition.unlock()}
        while items.count == capacity{
            condition.wait()
        }
        items.append(item)
        condition.broadcast()
    }
    
    func dequeue()->T? {
        condition.lock()
        defer {condition.unlock()}
        while items.isEmpty && !closed{
            condition.wait()
        }
        if items.isEmpty {
         return nil
        }
        let item = items.removeFirst()
        condition.broadcast()
        return item
    }
    
    func close() {
        condition.lock()
        defer { condition.unlock() }
        closed = true
        condition.broadcast()
    }
}
