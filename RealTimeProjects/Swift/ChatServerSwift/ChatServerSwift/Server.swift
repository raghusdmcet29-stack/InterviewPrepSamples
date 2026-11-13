//
//  Server.swift
//  ChatServerSwift
//
//  Created by Anussha on 13/11/26.
//

import Darwin
import Foundation

enum ServerError : Error {
    case socketFailed
    case bindFailed
    case listenFailed
}

class Server {
    var fds : [pollfd] = []
    var clientRoom : [Int32 : String] = [:]
    var clientBuffers: [Int32: [UInt8]] = [:]  // socket -> bytes received but not yet a full line
    var clientOutput: [Int32: [UInt8]] = [:]   // socket -> bytes waiting to be sent
    let listener : Int32

    init(port: UInt16) throws {
        listener = socket(AF_INET, SOCK_STREAM, 0)
        guard listener >= 0 else {
            throw ServerError.socketFailed
        }

        var address = sockaddr_in()
        address.sin_family = sa_family_t(AF_INET)
        address.sin_addr.s_addr = INADDR_ANY
        address.sin_port = port.bigEndian

        let bindResult = withUnsafePointer(to: address) { pointer in
            pointer.withMemoryRebound(to: sockaddr.self, capacity: 1) { addr in
                bind(listener, addr, socklen_t(MemoryLayout<sockaddr_in>.size))
            }
        }

        guard bindResult == 0 else {
            throw ServerError.bindFailed
        }

        guard listen(listener, 5) == 0 else {
            throw ServerError.listenFailed
        }

        fds.append(pollfd(fd: listener, events: Int16(POLLIN), revents: 0))
    }

    func run() {
        while true {
            let ready = poll(&fds, nfds_t(fds.count), -1)
            if ready < 0 {
                print("poll failed")
                break
            }

            // Slot 0 is the listener. If it's ready, a client is waiting.
            if (fds[0].revents & Int16(POLLIN)) != 0 {
                acceptClient()
            }

            // Slots 1 and up are clients
            var i = 1
            while i < fds.count {
                let fd = fds[i].fd
                let revents = fds[i].revents
                if (revents & Int16(POLLIN)) != 0 {
                    if !readFromClient(fd) {
                        disconnect(at: i)
                        continue   // don't add 1: the next client has shifted into slot i
                    }
                    handleLines(fd)
                }
                if (revents & Int16(POLLOUT)) != 0 {
                    flushOutput(fd)
                }
                i += 1
            }
        }
    }

    func disconnect(at index: Int) {
        let fd = fds[index].fd
        close(fd)
        clientBuffers.removeValue(forKey: fd)
        clientRoom.removeValue(forKey: fd)
        clientOutput.removeValue(forKey: fd)
        fds.remove(at: index)
        print("client \(fd) disconnected")
    }

    func acceptClient() {
        let client = accept(listener, nil, nil)
        guard client >= 0 else {
            print("accept failed")
            return
        }
        guard setNonBlocking(client) else {
            print("fcntl failed")
            close(client)
            return
        }
        fds.append(pollfd(fd: client, events: Int16(POLLIN), revents: 0))
        print("client \(client) connected")
    }

    func setNonBlocking(_ fd: Int32) -> Bool {
        let flags = fcntl(fd, F_GETFL)
        if flags < 0 {
            return false
        }
        return fcntl(fd, F_SETFL, flags | O_NONBLOCK) == 0
    }

    func readFromClient(_ fd: Int32) -> Bool {
        var buf = [UInt8](repeating: 0, count: 1024)
        let n = read(fd, &buf, buf.count)
        if n < 0 && (errno == EAGAIN || errno == EWOULDBLOCK) {
            return true    // nothing to read yet, not a disconnect
        }
        if n <= 0 {
            return false   // 0 means the client closed; negative means an error
        }
        clientBuffers[fd, default: []].append(contentsOf: buf[0..<n])
        return true
    }

    func handleLines(_ fd: Int32) {
        guard var pending = clientBuffers[fd] else {
            return
        }
        // 10 is the byte for "\n"
        while let newline = pending.firstIndex(of: 10) {
            let line = String(decoding: pending[0..<newline], as: UTF8.self)
            pending.removeSubrange(0...newline)
            handleLine(fd, line)
        }
        clientBuffers[fd] = pending
    }

    func handleLine(_ fd: Int32, _ line: String) {
        if line == "/join" || line.hasPrefix("/join ") {
            let room = line.dropFirst(6).trimmingCharacters(in: .whitespaces)
            if room.isEmpty {
                queue(fd, "usage: /join <name>")
                return
            }
            joinRoom(&clientRoom, fd, String(room))
            queue(fd, "joined \(room)")
            return
        }
        if line == "/leave" {
            leaveRoom(&clientRoom, fd)
            queue(fd, "left the room")
            return
        }
        if !inRoom(clientRoom, fd) {
            queue(fd, "join a room first with /join <name>")
            return
        }
        let labeled = "client \(fd): \(line)"
        print(labeled)
        broadcast(fd, labeled)
    }

    func queue(_ fd: Int32, _ text: String) {
        clientOutput[fd, default: []].append(contentsOf: Array((text + "\n").utf8))
        if let index = fds.firstIndex(where: { $0.fd == fd }) {
            fds[index].events |= Int16(POLLOUT)   // ask poll() to tell us when this socket can take data
        }
    }

    func flushOutput(_ fd: Int32) {
        guard var out = clientOutput[fd], !out.isEmpty else {
            return
        }
        let sent = out.withUnsafeBufferPointer { write(fd, $0.baseAddress, $0.count) }
        if sent > 0 {
            out.removeFirst(sent)
        }
        clientOutput[fd] = out
        if out.isEmpty, let index = fds.firstIndex(where: { $0.fd == fd }) {
            fds[index].events &= ~Int16(POLLOUT)   // nothing left to send; stop watching for writability
        }
    }

    func broadcast(_ sender: Int32, _ message: String) {
        for entry in fds.dropFirst() where entry.fd != sender {
            if sameRoom(clientRoom, entry.fd, sender) {
                queue(entry.fd, message)
            }
        }
    }
}


