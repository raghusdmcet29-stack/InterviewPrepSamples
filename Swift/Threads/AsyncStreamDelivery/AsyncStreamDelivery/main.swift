//
//  main.swift
//  AsyncStreamDelivery
//
//  Created by Anussha on 13/10/26.
//
// with out waitng with external sleep. consumer sends signal stream finish
import Foundation

let done = DispatchSemaphore(value: 0)

let stream = AsyncStream<Int> { continuation in
    Task {
        for i in 1...3 {
            try? await Task.sleep(nanoseconds: 1_000_000_000)
            continuation.yield(i)
        }
        continuation.finish()
    }
}

Task {
    for await value in stream {
        print("Got \(value)")
    }
    print("Stream finished")
    done.signal()
}

done.wait()


// wiating 2 seconds added belwo. if task yild not finsihed in 2 second we will lose data
/*import Foundation

let stream = AsyncStream<Int> { continuation in
    for i in 1...3 {
        continuation.yield(i)
    }
    continuation.finish()
}

Task {
    for await value in stream {
        print("Got \(value)")
    }
    print("Stream finished")
}

sleep(2)
*/
