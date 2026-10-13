//
//  main.swift
//  StreamingStep3-PartialFailure
//
//  Created by Anussha on 13/10/26.
//


import Foundation

struct StreamResult {
    let text: String
    let complete: Bool
}

func readStream(failAt: Int?) -> StreamResult {
    let pieces = ["Hel", "lo", "!"]
    var full = ""
    for (i, piece) in pieces.enumerated() {
        if i == failAt {
            return StreamResult(text: full, complete: false)
        }
        full += piece
    }
    return StreamResult(text: full, complete: true)
}

let good = readStream(failAt: nil)
print("\(good.text), complete: \(good.complete)")

let broken = readStream(failAt: 1)
print("\(broken.text), complete: \(broken.complete)")


/*
import Foundation

func readStream(failAt: Int?) -> String {
    let pieces = ["Hel", "lo", "!"]
    var full = ""
    for (i, piece) in pieces.enumerated() {
        if i == failAt {
            print("stream broke before piece \(i)")
            break
        }
        full += piece
    }
    return full
}

print(readStream(failAt: nil))
print(readStream(failAt: 2))
print(readStream(failAt: 1))

*/
