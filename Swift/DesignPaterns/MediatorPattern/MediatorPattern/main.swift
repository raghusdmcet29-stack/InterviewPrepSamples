//
//  main.swift
//  MediatorPattern
//
//  Created by Anussha on 17/09/26.
//

import Foundation

protocol ChatMediator: AnyObject {
    func send(message: String, from sender: User)
    func register(user: User)
}

class User {
    let name: String
    weak var mediator: ChatMediator?

    init(name: String, mediator: ChatMediator) {
        self.name = name
        self.mediator = mediator
    }

    func send(_ message: String) {
        mediator?.send(message: message, from: self)
    }

    func receive(_ message: String, from senderName: String) {
        print("\(name) received: \(senderName): \(message)")
    }
}

class ChatRoomMediator: ChatMediator {
    private var users: [User] = []

    func register(user: User) {
        users.append(user)
    }

    func send(message: String, from sender: User) {
        for user in users {
            if user !== sender {
                user.receive(message, from: sender.name)
            }
        }
    }
}

let mediator = ChatRoomMediator()

let alice = User(name: "Alice", mediator: mediator)
let bob = User(name: "Bob", mediator: mediator)
let charlie = User(name: "Charlie", mediator: mediator)

mediator.register(user: alice)
mediator.register(user: bob)
mediator.register(user: charlie)

alice.send("Hello everyone")
