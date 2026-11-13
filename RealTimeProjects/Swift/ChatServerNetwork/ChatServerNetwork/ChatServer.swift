//
//  ChatServer.swift
//  ChatServerNetwork
//
//  Created by Anussha on 13/11/26.
//

import Network
import Foundation

class ChatServer {
    let listener: NWListener
    var buffers: [ObjectIdentifier: Data] = [:]   // connection -> bytes not yet a full line
    var clientRoom: [ObjectIdentifier: String] = [:]   // connection -> room name
    var numbers: [ObjectIdentifier: Int] = [:]   // connection -> client number
    var nextNumber = 1
    var connections: [ObjectIdentifier: NWConnection] = [:]   // connection -> the connection itself
    
    init() throws {
        listener = try NWListener(using: .tcp, on: 8080)
    }

    func start() {
        listener.stateUpdateHandler = { state in
           print("listener state: \(state)")
        }
        listener.newConnectionHandler = { connection in
            let key = ObjectIdentifier(connection)
            self.numbers[key] = self.nextNumber
            print("client \(self.nextNumber) connected")
            self.nextNumber += 1
            self.connections[key] = connection
            connection.start(queue: .main)
            self.receiveLoop(connection)
        }
        listener.start(queue: .main)
        print("listening on port 8080")
    }

    func receiveLoop(_ connection: NWConnection) {
        connection.receive(minimumIncompleteLength: 1, maximumLength: 1024) { data, _, isComplete, error in
            let key = ObjectIdentifier(connection)
            if let data = data, !data.isEmpty {
                self.buffers[key, default: Data()].append(data)
                if !self.splitLines(connection) {
                    print("client \(self.numbers[key] ?? 0) sent a line that is too long, disconnecting")
                    self.removeClient(key)
                    connection.cancel()
                    return
                }
            }
            if isComplete || error != nil {
                print("connection closed")
                self.removeClient(key)
                connection.cancel()
                return
            }
            self.receiveLoop(connection)
        }
    }
    
    func broadcast(_ sender: ObjectIdentifier, _ message: String) {
        let myRoom = clientRoom[sender] ?? ""
        for (other, connection) in connections where other != sender && clientRoom[other] == myRoom {
            send(connection, message)
        }
    }
    
    func splitLines(_ connection: NWConnection) -> Bool {
        let key = ObjectIdentifier(connection)
        guard var pending = buffers[key] else {
            return true
        }
        
        // 10 is the byte for "\n"
        while let newline = pending.firstIndex(of: 10) {
            if newline - pending.startIndex > 1024 {
                return false
            }
            let line = String(decoding: pending[pending.startIndex..<newline], as: UTF8.self)
            pending.removeSubrange(pending.startIndex...newline)
            handleLine(connection, line)
        }
        if pending.count > 1024 {
            return false   // unfinished text is already too long
        }
        buffers[key] = pending
        return true
    }
    
    func send(_ connection: NWConnection, _ text: String) {
            connection.send(content: Data((text + "\n").utf8), completion: .contentProcessed { _ in })
        }
    
    func handleLine(_ connection: NWConnection, _ line: String) {
        let key = ObjectIdentifier(connection)
        if line == "/join" || line.hasPrefix("/join ") {
            let room = line.dropFirst(6).trimmingCharacters(in: .whitespaces)
            if room.isEmpty {
                send(connection, "usage: /join <name>")
                return
            }
            clientRoom[key] = String(room)
            send(connection, "joined \(room)")
            return
        }
        if clientRoom[key] == nil {
           send(connection, "join a room first with /join <name>")
            return
        }
        if line == "/leave" {
            clientRoom.removeValue(forKey: key)
            send(connection, "left the room")
            return
        }
        if line == "/who" {
            let myRoom = clientRoom[key] ?? ""
            var numbersInRoom: [Int] = []
            for (other, room) in clientRoom where room == myRoom {
                numbersInRoom.append(numbers[other] ?? 0)
            }
            let names = numbersInRoom.sorted().map { "client \($0)" }
            send(connection, "in \(myRoom): " + names.joined(separator: " "))
            return
        }
        let labeled = "client \(numbers[key] ?? 0): \(line)"
        print(labeled)
        broadcast(key, labeled)
    }
    
    func removeClient(_ key: ObjectIdentifier) {
        buffers.removeValue(forKey: key)
        clientRoom.removeValue(forKey: key)
        numbers.removeValue(forKey: key)
        connections.removeValue(forKey: key)
    }
}
