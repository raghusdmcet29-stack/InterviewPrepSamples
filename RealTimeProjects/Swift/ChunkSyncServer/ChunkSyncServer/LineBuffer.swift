//
//  LineBuffer.swift
//  ChunkSyncServer
//
//  Created by Anussha on 23/11/26.
// Collects text as it arrives and hands back complete lines.
import Foundation

struct LineBuffer {
    private var pending =  Data()

    mutating func append(_ bytes: Data) {
            pending.append(bytes)
        }

    mutating func nextLine() -> String? {
        guard let newlineIndex = pending.firstIndex(of: UInt8(ascii: "\n")) else {
            return nil
        }
        let line = String(decoding: pending[..<newlineIndex], as: UTF8.self)
        pending = Data(pending[pending.index(after: newlineIndex)...])
        return line
    }
    
    // Returns exactly `count` bytes once that many have arrived, otherwise nil.
    mutating func nextBytes(count: Int) -> Data? {
        guard pending.count >= count else {
            return nil
        }
        let bytes = Data(pending.prefix(count))
        pending = Data(pending.dropFirst(count))
        return bytes
    }
}


