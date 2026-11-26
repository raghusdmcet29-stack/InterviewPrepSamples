//
//  Protocol.swift
//  ChunkSyncServer
//
//  Created by Anussha on 23/11/26.
//
import Foundation
import CryptoKit

let serverPort: UInt16 = 9100
let chunkSize = 4096
let respOK = "OK"
let respErr = "ERR"
let respHaveIt = "HAVE_IT"
let respNeed = "NEED"

enum Command {
    case have(fingerprint: String)
    case chunk(fingerprint: String, length: Int)
    case begin(name: String)
    case append(name: String, fingerprint: String)
    case commit(name: String)
}

// SHA-256 of the bytes, as 64 lowercase hex characters.
func fingerprint(of data: Data) -> String {
    SHA256.hash(data: data).map { String(format: "%02x", $0) }.joined()
}

func isValidFingerprint(_ text: String) -> Bool {
    text.count == 64 && text.allSatisfy { $0.isHexDigit }
}

// Blocks names like "../x" or "/etc/x" so files stay in their folder.
func isSafeName(_ name: String) -> Bool {
    !name.isEmpty && name.count <= 255 && !name.contains("/") && !name.hasPrefix(".")
}

// Turns one text line into a Command, or nil if the line is invalid.
func parseCommand(_ line: String) -> Command? {
    let parts = line.split(separator: " ").map(String.init)
    guard let verb = parts.first else { return nil }

    switch verb {
    case "HAVE":
        guard parts.count == 2, isValidFingerprint(parts[1]) else { return nil }
        return .have(fingerprint: parts[1])

    case "CHUNK":
        guard parts.count == 3, isValidFingerprint(parts[1]),
              let length = Int(parts[2]), (1...chunkSize).contains(length) else { return nil }
        return .chunk(fingerprint: parts[1], length: length)

    case "BEGIN":
        guard parts.count == 2, isSafeName(parts[1]) else { return nil }
        return .begin(name: parts[1])

    case "APPEND":
        guard parts.count == 3, isSafeName(parts[1]), isValidFingerprint(parts[2]) else { return nil }
        return .append(name: parts[1], fingerprint: parts[2])

    case "COMMIT":
        guard parts.count == 2, isSafeName(parts[1]) else { return nil }
        return .commit(name: parts[1])

    default:
        return nil
    }
}
