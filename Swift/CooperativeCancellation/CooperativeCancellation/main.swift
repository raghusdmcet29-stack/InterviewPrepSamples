//
//  main.swift
//  CooperativeCancellation
//
//  Created by Anussha on 28/08/26.
//

import Foundation

func stubbornWork() async -> String{
    for i in 1...10{
        // deliberately NOT checking cancellation
        try? await Task.sleep(nanoseconds: 300_000_000) //0.3 seconds
        print("stubbornWork tick \(i)")
    }
    return "stubbornWork finished"
}

func politeWork() async throws ->String{
    for i in 1...10 {
        try Task.checkCancellation() // throws CancellationError if cancelled
        try await Task.sleep(nanoseconds: 300_000_000)
        print("politeWork tick \(i)")
    }
    return "politeWork finished"
}

func fetchFast() async throws -> String {
    try await Task.sleep(nanoseconds: 500_000_000) // 0.5s
    return "fast result"
}

func fetchSlowAndFails() async throws -> String {
    try await Task.sleep(nanoseconds: 200_000_000) // 0.2s — fails quickly
    throw MyError.sourceFailed
}

func fetchSlowPolite() async throws -> String {
    for i in 1...10 {
        try Task.checkCancellation()
        try await Task.sleep(nanoseconds: 300_000_000)
        print("fetchSlowPolite tick \(i)")
    }
    return "slow polite result"
}

enum MyError: Error {
    case sourceFailed
}

let task = Task {
    await stubbornWork()
}

try? await Task.sleep(nanoseconds: 500_000_000) // let it tick a couple times
task.cancel()
print("Cancelled the task")

let result = await task.value
print("Result: \(result)")

let task2 = Task {
    try await politeWork()
}

try? await Task.sleep(nanoseconds: 500_000_000)
task2.cancel()
print("Cancelled task2")

do {
    let result = try await task2.value
    print("Result: \(result)")
} catch {
    print("politeWork threw: \(error)")
}

func fetchAll() async throws -> (String, String, String) {
    async let a = fetchFast()
    async let b = fetchSlowAndFails()
    async let c = fetchSlowPolite()

    return try await (a, b, c)
}

do {
    let results = try await fetchAll()
    print("All succeeded: \(results)")
} catch {
    print("fetchAll threw: \(error)")
}
