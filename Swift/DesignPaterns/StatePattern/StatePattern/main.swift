//
//  main.swift
//  StatePattern
//
//  Created by Anussha on 07/09/26.
//

import Foundation

protocol ConnectionState {
    func open() -> ConnectionState
    func receiveSyn() -> ConnectionState
    func establish() -> ConnectionState
    func close() -> ConnectionState
    var description: String { get }
}


struct ClosedState: ConnectionState {
    func open() -> ConnectionState { ListenState() }
    func receiveSyn() -> ConnectionState { self }      // invalid from here, stay put
    func establish() -> ConnectionState { self }
    func close() -> ConnectionState { self }
    var description: String { "Closed" }
}

struct ListenState: ConnectionState {
    func open() -> ConnectionState { self }
    func receiveSyn() -> ConnectionState { SynReceivedState() }
    func establish() -> ConnectionState { self }
    func close() -> ConnectionState { ClosedState() }
    var description: String { "Listen" }
}

struct SynReceivedState: ConnectionState {
    func open() -> ConnectionState { self }
    func receiveSyn() -> ConnectionState { self }
    func establish() -> ConnectionState { EstablishedState() }
    func close() -> ConnectionState { ClosedState() }
    var description: String { "SynReceived" }
}

struct EstablishedState: ConnectionState {
    func open() -> ConnectionState { self }
    func receiveSyn() -> ConnectionState { self }
    func establish() -> ConnectionState { self }
    func close() -> ConnectionState { ClosedState() }
    var description: String { "Established" }
}

final class Connection {
    private(set) var state: ConnectionState = ClosedState()

    func open() {
        state = state.open()
    }

    func receiveSyn() {
        state = state.receiveSyn()
    }

    func establish() {
        state = state.establish()
    }

    func close() {
        state = state.close()
    }
}


let conn = Connection()
print(conn.state.description)   // Closed

conn.open()
print(conn.state.description)   // Listen

conn.receiveSyn()
print(conn.state.description)   // SynReceived

conn.establish()
print(conn.state.description)   // Established

conn.receiveSyn()                // invalid here — should no-op
print(conn.state.description)   // Established (unchanged)

conn.close()
print(conn.state.description)   // Closed

let connA = Connection()
let connB = Connection()

connA.open()
connA.receiveSyn()

print("A:", connA.state.description)  // SynReceived
print("B:", connB.state.description)  // Closed — untouched
