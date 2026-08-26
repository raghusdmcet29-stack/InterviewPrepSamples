//
//  main.swift
//  AsyncRetryDemo
//
//  Created by Anussha on 26/08/26.
//

import Foundation

enum RetryError : Error {
    case allAttemptsFailed(lastError : Error)
}

func retry<T>(
    maxAttempts : Int,
    initialDelay : Duration = .microseconds(200),
    operation : @escaping () async throws -> T
)async throws -> T{
    var attempt = 1
    var delay = initialDelay
    
    while true {
        do {
            return try await operation()
        }catch{
            if attempt >= maxAttempts{
                throw RetryError.allAttemptsFailed(lastError: error)
            }
            print("Attempt \(attempt) failed: \(error). Retrying in \(delay)...")
            try await Task.sleep(for: delay)
            delay *= 2
            attempt += 1
        }
    }
}

actor FlakyCounter {
    private var attempts = 0
    func tryOperation() throws -> String{
        attempts += 1
        if attempts < 3{
            throw NSError(domain: "FlakyError", code: attempts, userInfo: nil)
        }
        return "Success on attempt \(attempts)"
    }
}

let counter = FlakyCounter()

Task {
    do {
        let result = try await retry(maxAttempts: 5){
            try await counter.tryOperation()
        }
        print("Final result:", result)
        
    }catch {
        print("Retry exhausted:", error)
    }
}

// Keep the command-line program alive to let the async Task finish
RunLoop.main.run(until: Date().addingTimeInterval(3))
