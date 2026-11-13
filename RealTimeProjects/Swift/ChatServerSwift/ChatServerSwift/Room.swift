//
//  Room.swift
//  ChatServerSwift
//
//  Created by Anussha on 13/11/26.
//

import Foundation

// Room.swift

func sameRoom(_ clientRoom: [Int32: String], _ a: Int32, _ b: Int32) -> Bool {
    guard let roomA = clientRoom[a], let roomB = clientRoom[b] else {
        return false
    }
    return !roomA.isEmpty && roomA == roomB
}

func inRoom(_ clientRoom: [Int32: String], _ client: Int32) -> Bool {
    guard let room = clientRoom[client] else {
        return false
    }
    return !room.isEmpty
}

func joinRoom(_ clientRoom: inout [Int32: String], _ client: Int32, _ room: String) {
    clientRoom[client] = room
}

func leaveRoom(_ clientRoom: inout [Int32: String], _ client: Int32) {
    clientRoom[client] = ""
}
