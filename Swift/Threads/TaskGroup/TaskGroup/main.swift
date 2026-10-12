//
//  main.swift
//  TaskGroup
//
//  Created by Anussha on 12/10/26.
//

// Task group with failed task also in results

import Foundation

struct JobError: Error {
    let name: String
}

func job(_ name: String, seconds: UInt64, fail: Bool = false) async throws -> String {
    try await Task.sleep(nanoseconds: seconds * 1_000_000_000)
    if fail {
        throw JobError(name: name)
    }
    print("\(name) finished")
    return name
}

Task {
    do {
        let results = try await withThrowingTaskGroup(of: String.self) { group in
            group.addTask { try await job("J1", seconds: 3) }
            group.addTask { try await job("J2", seconds: 1, fail: true) }
            group.addTask { try await job("J3", seconds: 2) }
            var collected: [String] = []
            for try await r in group {
                collected.append(r)
            }
            return collected
        }
        print("Results: \(results)")
    } catch {
        print("Group failed: \(error)")
    }
    exit(0)
}

RunLoop.main.run()
// sorted order results of task group

/*import Foundation

func job(_ name: String, seconds: UInt64) async -> String {
    try? await Task.sleep(nanoseconds: seconds * 1_000_000_000)
    print("\(name) finished")
    return name
}

Task {
    let results = await withTaskGroup(of: (Int, String).self) { group in
        group.addTask { (0, await job("J1", seconds: 3)) }
        group.addTask { (1, await job("J2", seconds: 1)) }
        group.addTask { (2, await job("J3", seconds: 2)) }
        var collected: [(Int, String)] = []
        for await r in group {
            collected.append(r)
        }
        return collected.sorted { $0.0 < $1.0 }.map { $0.1 }
    }
    print("Results: \(results)")
    exit(0)
}

RunLoop.main.run()
*/
// non ordered results of task group
/*
import Foundation

func job(_ name: String, seconds: UInt64) async -> String {
    try? await Task.sleep(nanoseconds: seconds * 1_000_000_000)
    print("\(name) finished")
    return name
}

Task {
    let results = await withTaskGroup(of: String.self) { group in
        group.addTask { await job("J1", seconds: 3) }
        group.addTask { await job("J2", seconds: 1) }
        group.addTask { await job("J3", seconds: 2) }
        var collected: [String] = []
        for await r in group {
            collected.append(r)
        }
        return collected
    }
    print("Results: \(results)")
    exit(0)
}

RunLoop.main.run()
*/
