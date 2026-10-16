//
//  TokenBucket.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

import Foundation

final class TokenBucket{
    private let capacity : Double
    private let refillPerSecond : Double
    private var tokens : Double
    private var lastRefill : Date
    private let condition = NSCondition()
    
    init(capacity: Int, refillPerSecond: Double) {
         self.capacity = Double(capacity)
         self.refillPerSecond = refillPerSecond
         self.tokens = Double(capacity)
         self.lastRefill = Date()
    }
    
    // Blocks until one token is available, then takes it
    
    func take(){
        condition.lock()
        while true {
            refill()
            if tokens >= 1 {
                tokens -= 1
                break
            }
            condition.wait(until: Date().addingTimeInterval(0.01))
        }
        condition.unlock()
    }
    
    // Caller must hold the lock
    private func refill() {
        let now = Date()
        let elapsed = now.timeIntervalSince(lastRefill)
        tokens = min(capacity, tokens + elapsed * refillPerSecond)
        lastRefill = now
    }
}
