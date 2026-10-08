//
//  main.swift
//  AsyncAwait
//
//  Created by Anussha on 08/10/26.
//

import Foundation

func fetchNumber() async -> Int {
    try? await Task.sleep(nanoseconds: 1_000_000_000)
    return 7
}


print("Start")
let task = Task {
    let n = await fetchNumber()
    print("Task got \(n)")
    return n
}
print("Main continues")
let result = await task.value
print("Result \(result)")

/*
print("Start")
async let a = fetchNumber()
async let b = fetchNumber()
let total = await a + b
print("Got \(total)")

*/

/*print("Start")
let a = await fetchNumber()
let b = await fetchNumber()
print("Got \(a) and \(b)")
*/


/*print("Start")
let n = await fetchNumber()
print("Got \(n)")
*/
