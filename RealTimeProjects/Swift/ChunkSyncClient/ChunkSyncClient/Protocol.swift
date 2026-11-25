//
//  Protocol.swift
//  ChunkSyncClient
//
//  Created by Anussha on 25/11/26.
//

import Foundation

// Shared rules between client and server.

import CryptoKit
import Foundation

let chunkSize = 4096
let serverPort: UInt16 = 9100
let cmdHave = "HAVE"
let cmdHaveIt = "HAVE_IT"
let cmdNeed = "NEED"
let cmdChunk = "CHUNK"
let cmdCommit = "COMMIT"
let respOK = "OK"
let respErr = "ERR"
let fingerprintLength = 64

func isValidFingerprint(_ value: String) -> Bool {
    return value.count == fingerprintLength
}

func isHexCharacter(_ c: Character) -> Bool {
    return c.isHexDigit
}

func isValidHexFingerprint(_ value: String) -> Bool {
    return isValidFingerprint(value) && value.allSatisfy(isHexCharacter)
}

func fingerprint(of data: Data) -> String {
    let digest = SHA256.hash(data: data)
    return digest.map { String(format: "%02x", $0) }.joined()
}

func splitIntoChunks(_ data: Data) -> [Data] {
    var chunks: [Data] = []
    var start = 0
    while start < data.count {
        let end = min(start + chunkSize, data.count)
        chunks.append(data.subdata(in: start..<end))
        start = end
    }
    return chunks
}

func isValidHaveLine(_ line: String) -> Bool {
    let parts = line.split(separator: " ")
    guard parts.count == 2 else { return false }
    guard String(parts[0]) == cmdHave else { return false }
    return isValidHexFingerprint(String(parts[1]))
}
