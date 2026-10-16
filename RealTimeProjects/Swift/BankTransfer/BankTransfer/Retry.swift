//
//  Retry.swift
//  BankTransfer
//
//  Created by Anussha on 16/10/26.
//

import Foundation

// Tries an operation up to maxAttempts times.
// Waits baseDelay after the first failure, then doubles the wait each time.
func withRetry(maxAttempts: Int, baseDelay: TimeInterval, operation: () -> Bool) -> Bool {
    var delay = baseDelay
    for attempt in 1...maxAttempts{
        if operation(){
            return true
        }
        if attempt < maxAttempts{
            Thread.sleep(forTimeInterval: delay)
            delay *= 2
        }
    }
    return false
}
